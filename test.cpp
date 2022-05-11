struct Foo { int i; float f; double d; };

int foo1(Foo f1) { return f.i; }

//Smeagle:
  //parameters: {
    //type:Foo, name:f,
    //fields:[
      //{name:i, type:int},
      //{name:f, type:float},
      //{name:d, type:double}
    //]
  //}

////float foo2(Foo f) { return f.f; }
////double foo3(Foo f) { return f.d + 1.0; }
