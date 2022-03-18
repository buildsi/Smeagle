# Development for Jim

First, pull the smeagle container I've built that has the callsite code and
(a not yet compiled) Smeagle:

```bash
$ docker pull ghcr.io/buildsi/smeagle-dev:callsites-2
```

If you are curious, this was built with the Smeagle add/callsites branch as follows:

```bash
docker build --build-arg fromimage=ghcr.io/dyninst/dyninst-branch-builder:callsites-2 -t smeagle .
```

The compilation is commented out in the Dockerfile because our recent changes broke a test that we haven't fixed yet.
Next clone the development branch with callsite stuffs.

```bash
$ git clone -b add/callsites https://github.com/buildsi/Smeagle
$ cd Smeagle
```

Run the smeagle container, binding `/code` so you can make changes locally and build in the container.

```bash
$ docker run -it -v $PWD:/code --entrypoint bash ghcr.io/buildsi/smeagle-dev:callsites-2
```

You'll be sitting in `/code`, which is the bound source code! We need to source the spack
environment and add dyninst to the path:

```bash
. /opt/spack/share/spack/setup-env.sh
spack load dyninst
```

You can then use `make` to compile everything:

```bash
$ make
```
This generates the Smeagle binary, which you can run on things. Here is an example with the code I sent you:

```cpp
int foo(int x) { return x + 1; }
double bar(double d) { return d + foo(d); };

inline int cube(int s){
    return s*s*s;
}

int start(double d) {
    double dd = bar(d);
    return dd + 1;
}
```

And then run:

```bash
$ ./build/standalone/Smeagle -l libfoo.so
```
