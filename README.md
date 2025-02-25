
`githooks` provide branch protection locally. Set the .githook/ directory with:
```bash
git config --local core.hooksPath .githooks
```

Install the required tools
```bash
make install
```

## Usage

Now, navigate to the main directory, and run:

```bash
make
```

This will build the current firmware. To upload, run:

```bash
make upload
```

There are a few other nice helper functions within the makefile. This will list them:

```bash
make help
```


## Licensing
This repository uses the MIT License, which covers the code and tools written for our robot firmware. See separate copyright/licensing information for the external libraries used in the repo (in the libaries/ and teensy4/ directories).
