Build executable in bin/:
```
make simulator
```

Build testbench in bin/:
```
make testbench
```

Run all tests and output csv
```
make test
```

Run all tests and output pretty table
```
make pretty_test
```

Clean everything:
```
make clean
```

## Dependencies

* https://github.com/mapbox/variant (In `include/`)

* https://github.com/kepkin/dev-shell-essentials 
  Not used for assessed code, just for pretty printing with make pretty_test.
  Only using a modified version of [highlight.sh](https://github.com/kepkin/dev-shell-essentials/blob/master/highlight.sh)
  in scripts/highlight.
