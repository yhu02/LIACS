# Mpark.Variant

> __C++17__ `std::variant` for __C++11__/__14__/__17__

Based on the original [Mpark.variant](https://github.com/mpark/variant) `v1.4.0`.
Our changes:
 1. Small adjustments to work for our GCC C++11.
 2. Changed build system to meson, (so it can be wrapped and distributed nicely to projects using this).


## Documentation

  - [cppreference.com](http://en.cppreference.com/w/cpp/utility/variant)
  - [eel.is/c++draft](http://eel.is/c++draft/variant)

## Integration

### Meson

Add a file `<project-root>/subprojects/mpark_variant.wrap`, with these contents:
```
[wrap-git]
url = https://github.com/Sebastiaan-Alvarez-Rodriguez/variant
revision = v1.4.0
```

Now you can add this to your `<project-root>/meson.build`:
```
mpark_variant = subproject('mpark_variant')
```

### Submodule

You can add `mpark/variant` as a submodule to your project.

```bash
git submodule add https://github.com/Sebastiaan-Alvarez-Rodriguez/variant.git subprojects/mpark_variant_vendored
```

In your `<project-root>/meson.build`, add:
```
mpark_variant = subproject('mpark_variant_vendored')
```

## Depending in Meson
After adding mpark variant to your project, simply add the following to depend on mpark_variant:
```
mpark_variant_dep = mpark_variant.get_variable('mpark_variant_dep')

// example showing how to use a dependency
libmylib = shared_library(
    'mylib',
    src_files,
    include_directories : inc,
    dependencies : [mpark_variant_dep, some_other_dep],
    install : true)
```

## Requirements

This library requires a standard conformant __C++11__ compiler.


## Credits
All credits for creating mpark variant to mpark.
We made some adjustments to better fit our build- and workflow and share it with the public.

## License

Changes distributed under the [Apache v2 License](LICENSE),
base work under the [Boost Software License, Version 1.0](sublicense/LICENSE.md).
