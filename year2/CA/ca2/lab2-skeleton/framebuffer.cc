/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    framebuffer.cc - a simple framebuffer for the simulator
 *
 * Copyright (C) 2019  Leiden University, The Netherlands.
 *
 * Authors:
 *     Koen Putman
 */


/* Framebuffer design notes
 * - Control interface for enable/disable and setting resolution/mode
 * - Different modes:
 *   0 - Y8/greyscale
 *   1 - 8-bit indexed
 *   2 - RGB332 (directly using SDL_PIXELFORMAT_RGB332)
 *   3 - RGB555 (directly using SDL_PIXELFORMAT_RGB555)
 *   4 - RGB24  (directly using SDL_PIXELFORMAT_RGB24)
 *   5 - RGBA32 (directly using SDL_PIXELFORMAT_RGBA888)
 * - Mode/resolution changes only work when screen is off to simplify design.
 * - Palette changes are always possible.
 * - The framebuffer is just a large chunk of memory uploaded to a texture
 *   and rendered to the screen. This isn't the most efficient method
 *   but it is very simple in its design. For the indexed/Y8 modes
 *   we translate it to RGBA8888 first.
 * - Two base memory addresses, one for control/palette which only accepts
 *   aligned word size writes.
 *   The other writes directly to the framebuffer memory we allocate.
 * - Refreshes happen every X bus cycles if any of the memory changed.
 *   Refresh frequency can be adjusted with up/down arrow keys.
 *
 * Relevant addresses:
 *  0x0000800 - Control interface/palette (word-size writes only)
 *                uint32_t enable;
 *                uint32_t mode;
 *                uint32_t resx;
 *                uint32_t resy;
 *                uint32_t palette[256];
 *  0x1000000 - Start of framebuffer (any write size)
 *              Placed higher up in memory so it probably won't interfere
 *              with anything else and so there's enough space to be able
 *              to support larger window sizes.
 */

#ifdef ENABLE_FRAMEBUFFER
#include "framebuffer.h"

#include <SDL.h>
#include <SDL_video.h>
#include <SDL_events.h>

/* PRIu64 on MSVC */
#include <cinttypes>

enum FBmode
{
  FBMODE_Y8 = 0,
  FBMODE_INDEXED,
  FBMODE_RGB332,
  FBMODE_RGB555,
  FBMODE_RGB24,
  FBMODE_RGBA32
};

/* We map most of our modes directly to SDL modes */
static int
sdl_mode_map[] =
{
  SDL_PIXELFORMAT_RGBA8888,//Y8
  SDL_PIXELFORMAT_RGBA8888,//INDEXED
  SDL_PIXELFORMAT_RGB332,  //RGB332
  SDL_PIXELFORMAT_RGB555,  //RGB555
  SDL_PIXELFORMAT_RGB24,   //RGB24
  SDL_PIXELFORMAT_RGBA8888 //RGBA32
};

/* bytes per pixel */
static const
int mem_mult[] =
{
  1, //Y8
  1, //INDEXED
  1, //RGB332
  2, //RGB555
  3, //RGB24
  4  //RGBA32
};


/*
 * RenderContext: Useful context for the current window and its contents.
 * Internal class.
 */

class RenderContext
{
  public:
    RenderContext(const uint32_t resx, const uint32_t resy,
                  const uint32_t mode);
    ~RenderContext();

    void redrawScreen();

    RenderContext(const RenderContext &) = delete;
    RenderContext &operator=(const RenderContext &) = delete;

    /* public members, will be accessed directly by class FrameBuffer. */
    SDL_Window   *window{};
    SDL_Renderer *renderer{};
    SDL_Texture  *texture{};
    uint8_t      *mem{};
    size_t        memsize{};

    bool active = false;
    bool changed = true;
    uint32_t mode;
    uint32_t resx;
    uint32_t resy;
};

static uint32_t palette[256];


RenderContext::RenderContext(const uint32_t resx, const uint32_t resy,
                             const uint32_t mode)
  : mode{ mode }, resx{ resx }, resy{ resy }
{
  /* Create a new window/renderer/texture */
  if (SDL_CreateWindowAndRenderer(resx, resy, 0,
                                  &window, &renderer))
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                   "Couldn't create window/renderer: %s", SDL_GetError());
      throw std::runtime_error("Error creating SDL window/renderer");
    }

  SDL_SetWindowTitle(window, "rv64-emu");
  texture = SDL_CreateTexture(renderer, sdl_mode_map[mode],
                              SDL_TEXTUREACCESS_STREAMING,
                              resx, resy);
  if (texture == NULL)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                   "Couldn't create texture: %s", SDL_GetError());
      throw std::runtime_error("Error creating texture");
    }

  memsize = resx * resy * mem_mult[mode];
  mem = (uint8_t *) calloc(memsize, sizeof(uint8_t));
  active  = true;
}

RenderContext::~RenderContext()
{
  /* Closes the window and deallocates everything */
  if (texture)
    SDL_DestroyTexture(texture);
  if (renderer)
    SDL_DestroyRenderer(renderer);
  if (window)
    SDL_DestroyWindow(window);
  if (mem)
    free(mem);
}


/* Update the texture and render it to the window */
void RenderContext::redrawScreen()
{
  switch(mode)
    {
      case FBMODE_RGB332:
      case FBMODE_RGB555:
      case FBMODE_RGB24:
      case FBMODE_RGBA32:
          SDL_UpdateTexture(texture, NULL, mem, resx * mem_mult[mode]);
          break;

      case FBMODE_Y8:
      case FBMODE_INDEXED:
          uint8_t * pixels;
          int pitch;
          SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
          for (uint32_t y = 0; y < resy; y++)
            for (uint32_t x = 0; x < resx; x++)
              {
                uint32_t * p = (uint32_t *)&pixels[pitch * y + x * sizeof(uint32_t)];
                uint8_t mval = mem[y * resx + x];
                uint32_t pval = 0;
                if (mode == FBMODE_Y8)
                  pval = mval << 24 | mval << 16 | mval << 8 | 0xff;
                else if (mode == FBMODE_INDEXED)
                  pval = palette[mval];
                *p = pval;
            }
          SDL_UnlockTexture(texture);
          break;
    }

  SDL_RenderCopy(renderer, texture, 0, 0);
  SDL_RenderPresent(renderer);
  changed = false;
}


/*
 * FrameBuffer implementation
 */

Framebuffer::Framebuffer(const MemAddress control_base,
                         const MemAddress framebuffer_base)
  : control_base{ control_base }, framebuffer_base{ framebuffer_base },
    context{}
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                   "Couldn't init SDL: %s", SDL_GetError());
      SDL_Quit();
      throw std::runtime_error("Error initialising SDL");
    }
}

Framebuffer::~Framebuffer()
{
  finished = true;
  processEvents(true);
  if (active_window)
    {
      SDL_SetWindowTitle(context->window, "Simulation over, press q/ESC to quit");
      while (active_window)
        {
          processEvents(true);
          SDL_Delay(200);
        }
    }

  if (context)
    context.reset(nullptr);

  SDL_Quit();
}

void
Framebuffer::processEvents(const bool redraw)
{
  if (not active_window)
    return;

  SDL_Event event;
  while (SDL_PollEvent(&event))
    {
      switch(event.type)
        {
          case SDL_KEYUP:
              switch (event.key.keysym.sym)
                {
                  case SDLK_ESCAPE:
                  case SDLK_q:
                    if (active_window)
                      {
                        context.reset(nullptr);

                        control.enable = 0;
                        active_window = false;
                      }
                    break;

                  case SDLK_UP:
                    if (update_freq > 10)
                      update_freq /= 10;
                    break;

                  case SDLK_DOWN:
                    if (update_freq < 10000000)
                      update_freq *= 10;
                    break;
                }
              break;

          default:
              break;
        }
    }

  /* Bail out if the window was just destroyed (due to quit command) */
  if (not active_window)
    return;

  /* Check if we need to update */
  if (!finished)
    {
      char tmp[256];
      snprintf(tmp, 256, "rv64-emu - %" PRIu64 " bus cycles/update", update_freq);
      SDL_SetWindowTitle(context->window, tmp);
    }

  if (context->changed || redraw)
    context->redrawScreen();
}

/* Because the control/palette/framebuffer sections are stored differently
 * we use this function to determine which of the sections an address is in
 * and also determine the offset. If called with size 0 it will only do the
 * range check (for contains), any higher size will throw exceptions.
 */
FBzone
Framebuffer::getZone(const MemAddress addr, const uint8_t size,
                     uint32_t * offset) const
{
  FBzone r = FBzone::INVALID;

  if (addr >= control_base &&
      addr + size <= control_base + sizeof(ControlInterface) + sizeof(palette))
    {
      if ((size > 0 && size != 4) || (size == 4 && addr % size != 0))
        throw IllegalAccess("Control/palette only support aligned 4 byte access");

      MemAddress pos = addr - control_base;
      if (pos < sizeof(ControlInterface))
        {
          r = FBzone::CONTROL;
          if (offset)
            *offset = pos;
        }
      else
        {
          r = FBzone::PALETTE;
          if (offset)
            *offset = pos - sizeof(ControlInterface);
        }
    }
  else if (not active_window && size == 0)
    return FBzone::INVALID;
  else if (not active_window && size > 0)
    throw IllegalAccess("Framebuffer device only accessible with an active window");
  /* From here onwards, an active window is guaranteed. */
  else if (addr >= framebuffer_base &&
           addr + size <= framebuffer_base + context->memsize)
    {
      if (offset)
        *offset = addr - framebuffer_base;
      r = FBzone::BUFFER;
    }

  return r;
}

bool
Framebuffer::contains(MemAddress addr) const
{
  return getZone(addr, 0, NULL) != FBzone::INVALID;
}

uint8_t
Framebuffer::readByte(MemAddress addr)
{
  uint32_t offset = 0;
  FBzone zone = getZone(addr, sizeof(uint8_t), &offset);
  if (zone == FBzone::INVALID)
    throw IllegalAccess("Illegal access on framebuffer");

  return context->mem[offset];
}

uint16_t
Framebuffer::readHalfWord(MemAddress addr)
{
  uint32_t offset = 0;
  FBzone zone = getZone(addr, sizeof(uint16_t), &offset);
  if (zone == FBzone::INVALID)
    throw IllegalAccess("Illegal halfword access on framebuffer");

  return *(uint16_t*)&context->mem[offset];
}

uint32_t
Framebuffer::readWord(MemAddress addr)
{
  uint32_t offset = 0;
  FBzone zone = getZone(addr, sizeof(uint32_t), &offset);

  switch(zone)
    {
      case FBzone::CONTROL:
          return ((uint32_t*)&control)[offset/sizeof(uint32_t)];

      case FBzone::PALETTE:
          return palette[offset/sizeof(uint32_t)];

      case FBzone::BUFFER:
          return *(uint32_t*)&context->mem[offset];

      default:
          throw IllegalAccess("Invalid word access on framebuffer");
    }
}

uint64_t
Framebuffer::readDoubleWord(MemAddress addr)
{
  uint32_t offset = 0;
  FBzone zone = getZone(addr, 8, &offset);
  if (zone == FBzone::INVALID)
    throw IllegalAccess("Illegal doubleword access on framebuffer");

  return *(uint64_t *)&context->mem[offset];
}

void
Framebuffer::writeByte(MemAddress addr, uint8_t value)
{
  uint32_t offset = 0;
  FBzone zone = getZone(addr, sizeof(uint8_t), &offset);
  if (zone == FBzone::INVALID)
    throw IllegalAccess("Illegal access on framebuffer");

  context->mem[offset] = value;
  context->changed = true;
}

void
Framebuffer::writeHalfWord(MemAddress addr, uint16_t value)
{
  uint32_t offset = 0;
  FBzone zone = getZone(addr, sizeof(uint16_t), &offset);
  if (zone == FBzone::INVALID)
    throw IllegalAccess("Illegal access on framebuffer");

  *(uint16_t*)&context->mem[offset] = value;
  context->changed = true;
}

void
Framebuffer::writeWord(MemAddress addr, uint32_t value)
{
  uint32_t offset = 0;
  FBzone zone = getZone(addr, sizeof(uint32_t), &offset);

  switch(zone)
    {
      case FBzone::CONTROL:
        if (offset == 0)
          {
            /* Turn the device on or off at address 0 */
            if (active_window && value == 0)
              {
                context.reset(nullptr);

                active_window = false;
                control.enable = 0;
              }
            else if (not active_window && value > 0)
              {
                context.reset(new RenderContext(control.resx, control.resy,
                                                control.mode));
                active_window = true;
                control.enable = 1;
              }
          }
        else
          {
            /* Writing directly to the struct */
            ((uint32_t*)&control)[offset/sizeof(uint32_t)] = value;
          }
        break;

      case FBzone::PALETTE:
        palette[offset/sizeof(uint32_t)] = value;
        if (active_window)
          context->changed = true;
        break;

      case FBzone::BUFFER:
        *(uint32_t*)&context->mem[offset] = value;
        context->changed = true;
        break;

      default:
        throw IllegalAccess("Invalid Word access on framebuffer");
    }
}

void
Framebuffer::writeDoubleWord(MemAddress addr, uint64_t value)
{
  uint32_t offset = 0;
  FBzone zone = getZone(addr, sizeof(uint64_t), &offset);
  if (zone == FBzone::INVALID)
    throw IllegalAccess("Illegal access on framebuffer");

  *(uint64_t*)&context->mem[offset] = value;
  context->changed = true;
}

void
Framebuffer::clockPulse()
{
  if (cycles_since_update > update_freq)
    {
      processEvents(true);
      cycles_since_update = 0;
    }
  ++cycles_since_update;
}

#endif
