/// <reference types="vitest" />
import { defineConfig } from 'vite';
import { defineConfig as defineTestingConfig } from 'vitest/config'
import { mergeConfig } from 'vite'
import react from '@vitejs/plugin-react'
import svgr from 'vite-plugin-svgr'
import Pages from 'vite-plugin-pages'
import tsconfigPaths from 'vite-tsconfig-paths'
import { VitePluginFonts } from 'vite-plugin-fonts'

const viteConfig = defineConfig({
    plugins: [
        svgr(),
        react(),
        Pages({
            importMode(filepath, _) {
                return (filepath.includes('editor') || filepath == '/') ? 'sync' : 'async'
            },
        }
        ),
        tsconfigPaths(),
        VitePluginFonts({
            google: {
                families: ['IBM Plex Mono'],
            },
        }),
    ],
})

const testingConfig = defineTestingConfig({
    test: {
        globals: true,
        environment: 'jsdom',
        setupFiles: ['./src/tests/setup.ts'],
    },
})

export default mergeConfig(viteConfig, testingConfig)
