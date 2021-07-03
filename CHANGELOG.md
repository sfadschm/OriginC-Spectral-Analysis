# Changelog

## 2021-07-03

**Important Changes:**

- Create Changelog :smile:
- The global method `convert` is now named `map`.
- The `MAP_4D_Linescan` functionality was moved from `analyse` to `map`.
- Dropped support for running `analyse` on whole workbooks as this is hard to handle and barely used.

**Fixed bugs:**

- `MAP_4D_Linescan` sets wrong scanning axis name in result sheet.

**Implemented enhancements:**

- `MAP_4D_Linescan` can now take a parameter `precision` to account for slightly varying coordinates over the map.

**Other Changes:**

- Disabled the `width` parameter for `MAP_4D_Linescan`, as it is not implemented yet.