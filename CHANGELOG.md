# Changelog

## v1.1.0 (2021-07-08)

**Caution: This version changes the installation method of the package!**

### 2021-07-08

**Implemented Enhancements:**
- Implement package building and build first opx package.
- Add exemplary plugin code and package.
- Add developer guide for plugin extensions.

### 2021-07-07

**Important Changes:**
- Renamed main file and moved sources to subfolder for packaging.
- Create true header files with associated .c files.

### 2021-07-06

**Other Changes:**
- Add badges and links in readme.

## v1.0.0 (2021-07-05)

### 2021-07-05

**Fixed Bugs:**
- 3D map import did set wrong intensity axis labels.

**Implemented Enhancements:**
- Refined and extended test data.

**Internal Changes:**
- Moved all source files into the `src` directory.
- Changed all filenames to lowercase.
- Introduced and applied clang-format style fixer.
- Introduce cppcheck for static analysis. 

### 2021-07-04

**Fixed Bugs:**
- Only show visible user labels in `analyze` and `correct`.

**Implemented Enhancements:**
- Added parameters `Skip Abscissae` and "Y Abscissa" to `analyze`.
- Extract units from user label name in `analyze`.

**Deprecations:**
- Deprecated function `analyse`, use function `analyze` instead.

**Internal Changes:**
- Replace `ANALYSE` by `ANALYZE` to unify spelling in American English.
- Fix license and file headers.

### 2021-07-03

**Important Changes:**
- Create Changelog :smile:
- The global method `convert` is now named `map`.
- The `MAP_4D_Linescan` functionality was moved from `analyse` to `map`.
- Dropped support for running `analyse` on whole workbooks as this is hard to handle and barely used.

**Fixed Bugs:**
- `MAP_4D_Linescan` sets wrong scanning axis name in result sheet.
- Disabled the `width` parameter for `MAP_4D_Linescan`, as it is not implemented yet.

**Implemented Enhancements:**
- `MAP_4D_Linescan` can now take a parameter `precision` to account for slightly varying coordinates over the map.

### 2021-07-02

**Implemented Enhancements:**
- Pre-select active layer in `correct` user dialog.

### 2021-07-01

**Important Changes:**
- Refactored user guide now to be found in `docs/UserGuide.pdf`.

**Fixed Bugs:**
- Remove empty values from string vectors in `MISC_arrayUnique`.
- Fixed `MAP_4D_Linescan` `width` calculation.

**Implemented Enhancements:**
- Add new method for constant background subtraction in `correct`.

### 2021-06-13

**Implemented Enhancements:**
- [DEV] Add new `timer()` method for debugging method runtime.

### 2021-05-31

**Fixed Bugs:**
- Fix sheet name lookup when creating matrix layer with `ORIGIN_createMl`.