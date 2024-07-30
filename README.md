# Welcome to NeXTCode Programming Language !
NeXTCode is an innovative programming language that combines the efficiency and speed of Mojo with the elegance and practicality of Ada. It's designed to be fast, safe, and suitable for both systems programming and rapid application development.

```ada
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
  
## Inspired From:
- Ada 83/2012 - Type System, Safety Features and Reliability
- Fortran 77 - Legacy Code Integration and Compiler Optimization
- Assembly - Embedded Systems, Systems Programming
- Machine Code - Manually adapting language for close-to-the-metal memory and hardware control.
- Objective-C/C++ - For memory and hardware allocation capabalities
- C/C++20 - The main programming language used
- ALGOL 68 (MALGOL and ALGEK) - Syntax, Expressions
- LLVM (Version 18.1.5) - For constructing compiler
- Makefile - A software construction tool

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
