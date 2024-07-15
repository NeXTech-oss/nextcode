# Welcome to NeXTCode Programming Language !
NeXTCode is an innovative programming language that combines the efficiency and speed of Mojo with the elegance and practicality of Ada. It's designed to be fast, safe, and suitable for both systems programming and rapid application development.

```
procedure Main is
   type Circle is record
      Radius : Float;
   end record;

   Pi : constant Float := 3.14;

   function Area(C : Circle) return Float is
   begin
      return Pi * C.Radius ** 2;
   end Area;

   procedure Print_Total_Area(Circles : array of Circle) is
      Total_Area : Float := 0.0;
   begin
      for C of Circles loop
         Total_Area := Total_Area + Area(C);
      end loop;
      Output("Total area: " & Float'Image(Total_Area));
   end Print_Total_Area;

   Circles : array(1 .. 2) of Circle := (Circle'(1.0), Circle'(2.0));
begin
   Print_Total_Area(Circles);
end Main;
```

## Killer Features of the Language

- Speed: Enjoy the performance of Mojo, making your applications run at lightning speed.
- Compilation: Supports both ahead-of-time and just-in-time compilation, similar to Dart, for flexibility in deployment.
- Null Safety: Borrowing from Swift, NeXTCode ensures null safety without the need for special syntax like "?" after data types.
- Expressions: NeXTCode adopts Kotlin and Rust's approach where most control-flow structures are expressions.
- Collections: Null-safe methods on collections are a feature, taking cues from Swift to prevent common errors.
- Memory Control: Offers close-to-the-metal memory and hardware control akin to Objective-C, with the safety of Go.
- Syntax: NeXTCode uses a Python and C/C++-based syntax that is familiar and easy to understand.
- Cross-Compiling: Seamlessly use C# and C/C++, R, Ruby, Fortran and many other's libraries within NeXTCode, thanks to its cross-compiling capabilities.
- Interop: Enjoy smooth interoperation with C++ code and libraries.
- Visibility and Accessibility: Introduces hybrid visibility and accessibility modifiers for classes and subroutines.
  
## Inspired From:
- Ada 83/2012 - Safety Features and Reliability
- Fortran 77 - Legacy Code Integration and Compiler Optimization
- Assembly - Embedded Systems, Systems Programming
- Machine Code - Manually adapting language for close-to-the-metal memory and hardware control.
- Objective-C/C++ - For memory and hardware allocation capabalities
- C/C++20 - The main programming language used
- Swift - Syntax, Type System, Expressions
- LLVM (Version 18.1.5) - For constructing compiler
- CMake - A software construction tool

## People behind the NeXTCode

- Tunjay Akbarli - Initial work - tunjayakbarli@it-gss.com
- Tural Ghuliev - Lead Architect - turalquliyev@it-gss.com
- Teymur Novruzov - Lead Architect - teymurnovruzov@it-gss.com
- Mohammed Samy El-Melegy - Compiler Engineer - mohammedsamy@it-gss.com
- Uzo Ochogu - Compiler Engineer - uzoochogu@it-gss.com
- Martins Iwuogor - Compiler Developer - martinsiwuogor@it-gss.com
- Maryna Rybalko - Publisizing - maryna.rybalko@it-gss.com

## Contributing
Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are greatly appreciated.

If you have a suggestion that would make NeXTCode better, please fork the repo and create a pull request. You can also simply open an issue with the tag “enhancement”.

Don’t forget to give the project a star! Thanks again!
