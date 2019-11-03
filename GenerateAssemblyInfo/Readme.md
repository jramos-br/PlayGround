# GenerateAssemblyInfo

This project demonstrates the automatic generation of the assembly attributes that would
normally be defined in `AssemblyInfo.cs`.

[The AssemblyInfo file](#the-assemblyinfo-file)<br/>
[The new csproj file format](#the-new-csproj-file-format)<br/>
[Auto-generated assembly attributes](#auto-generated-assembly-attributes)<br/>
[Assembly attributes mixed from two sources](#assembly-attributes-mixed-from-two-sources)<br/>
[See also](#see-also)<br/>
[License](#license)

## The AssemblyInfo file

The `AssemblyInfo.cs` class file holds application attributes about a Visual Studio
project that are applied at the assembly level like company, product, description,
version, etc.

These attributes can be modified two different ways:

- Open the project Properties Editor, select the Application tab and then click the
  Assembly Information button to open the Assembly Information dialog box. All of the
  information saved within the Assembly Information dialog box gets stored in the
  `AssemblyInfo.cs` file.
- Expand the Properties node in the Solution Explorer and open the file `AssemblyInfo.cs`.
  The attributes can be modified here for quicker editing instead of using the GUI editor.

A typical `AssemblyInfo.cs` file looks like the following:

```c#
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

// General Information about an assembly is controlled through the following
// set of attributes. Change these attribute values to modify the information
// associated with an assembly.
[assembly: AssemblyTitle("Health Control Application")]
[assembly: AssemblyDescription("")]
#if DEBUG
[assembly: AssemblyConfiguration("Debug")]
#else
[assembly: AssemblyConfiguration("Release")]
#endif
[assembly: AssemblyCompany("Springfield Vegan")]
[assembly: AssemblyProduct("Vegan Health")]
[assembly: AssemblyCopyright("Copyright (c) Springfield Vegan")]
[assembly: AssemblyTrademark("D'oh!")]
[assembly: AssemblyCulture("")]

// Setting ComVisible to false makes the types in this assembly not visible
// to COM components.  If you need to access a type in this assembly from
// COM, set the ComVisible attribute to true on that type.
[assembly: ComVisible(false)]

// The following GUID is for the ID of the typelib if this project is exposed to COM
[assembly: Guid("7f941ba5-6732-41aa-80e1-505206c4e894")]

// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version
//      Build Number
//      Revision
//
// You can specify all the values or you can default the Build and Revision Numbers
// by using the '*' as shown below:
// [assembly: AssemblyVersion("1.0.*")]
[assembly: AssemblyVersion("1.0.0.0")]
[assembly: AssemblyFileVersion("1.0.0.0")]
```

## The new csproj file format

Visual Studio 2017 introduced a [new csproj file format](https://docs.microsoft.com/en-us/dotnet/core/tools/csproj)
for C# projects. The new format is leaner, easier to read, and adds new features.

One of these features allows you to define assembly attributes such as title, company and
version as project properties defined in the csproj file. Assembly attributes that were
typically present in an `AssemblyInfo.cs` file are now automatically generated from these
properties.

A typical csproj file for a .NET Core 3.0 console application looks like the following:

```xml
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>netcoreapp3.0</TargetFramework>
  </PropertyGroup>
</Project>
```

Each assembly attribute has a property that control its content and another to disable its
generation as shown in the following table:

Attribute                             | Property             | Property to disable
------------------------------------- | -------------------- | ---------------------------------------------
AssemblyCompanyAttribute              | Company              | GenerateAssemblyCompanyAttribute
AssemblyConfigurationAttribute        | Configuration        | GenerateAssemblyConfigurationAttribute
AssemblyCopyrightAttribute            | Copyright            | GenerateAssemblyCopyrightAttribute
AssemblyDescriptionAttribute          | Description          | GenerateAssemblyDescriptionAttribute
AssemblyFileVersionAttribute          | FileVersion          | GenerateAssemblyFileVersionAttribute
AssemblyInformationalVersionAttribute | InformationalVersion | GenerateAssemblyInformationalVersionAttribute
AssemblyProductAttribute              | Product              | GenerateAssemblyProductAttribute
AssemblyTitleAttribute                | AssemblyTitle        | GenerateAssemblyTitleAttribute
AssemblyVersionAttribute              | AssemblyVersion      | GenerateAssemblyVersionAttribute
NeutralResourcesLanguageAttribute     | NeutralLanguage      | GenerateNeutralResourcesLanguageAttribute

Notes:

- AssemblyVersion and FileVersion default is to take the value of $(Version) without suffix.
  For example, if $(Version) is 1.2.3-beta.4, then the value would be 1.2.3.
- InformationalVersion defaults to the value of $(Version).
- InformationalVersion has $(SourceRevisionId) appended if the property is present. It can
  be disabled using IncludeSourceRevisionInInformationalVersion.
- Copyright and Description properties are also used for NuGet metadata.
- Configuration is shared with all the build process and set via the --configuration
  parameter of dotnet commands.

## Auto-generated assembly attributes

This is a csproj file for a .NET Core 3.0 console application with the properties
AssemblyTitle and Version defined.

```xml
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>netcoreapp3.0</TargetFramework>
    <AssemblyTitle>Health Control Application</AssemblyTitle>
    <Version>1.0.0.0</Version>
  </PropertyGroup>
</Project>
```

During the build process of the above csproj file, MSBuild automatically generates the
file `GenerateAssemblyInfo.AssemblyInfo.cs`, containing the AssemblyTitle and Version
attributes defined with the values from the csproj properties.

```c#
using System;
using System.Reflection;

[assembly: System.Reflection.AssemblyCompanyAttribute("GenerateAssemblyInfo")]
[assembly: System.Reflection.AssemblyConfigurationAttribute("Debug")]
[assembly: System.Reflection.AssemblyFileVersionAttribute("1.0.0.0")]
[assembly: System.Reflection.AssemblyInformationalVersionAttribute("1.0.0.0")]
[assembly: System.Reflection.AssemblyProductAttribute("GenerateAssemblyInfo")]
[assembly: System.Reflection.AssemblyTitleAttribute("Health Control Application")]
[assembly: System.Reflection.AssemblyVersionAttribute("1.0.0.0")]
```

Besides AssemblyTitle and Version, defined in the csproj, other assembly attributes are
automatically generated. To disable that, appropriate properties must be defined in the
csproj file. In the following example, the generation of Company and Product attributes
is disabled.

```xml
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>netcoreapp3.0</TargetFramework>
    <AssemblyTitle>Health Control Application</AssemblyTitle>
    <Version>1.0.0.0</Version>
    <GenerateAssemblyCompanyAttribute>false</GenerateAssemblyCompanyAttribute>
    <GenerateAssemblyProductAttribute>false</GenerateAssemblyProductAttribute>
  </PropertyGroup>
</Project>
```

The `GenerateAssemblyInfo.AssemblyInfo.cs` file looks like the following:

```c#
using System;
using System.Reflection;

[assembly: System.Reflection.AssemblyConfigurationAttribute("Debug")]
[assembly: System.Reflection.AssemblyFileVersionAttribute("1.0.0.0")]
[assembly: System.Reflection.AssemblyInformationalVersionAttribute("1.0.0.0")]
[assembly: System.Reflection.AssemblyTitleAttribute("Health Control Application")]
[assembly: System.Reflection.AssemblyVersionAttribute("1.0.0.0")]
```

You can control most of the assembly attributes generation with properties in csproj.
If you'd rather keep the attributes definition in `AssemblyInfo.cs` file, you can turn
off the automatic generation of assembly attributes.

```xml
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>netcoreapp3.0</TargetFramework>
    <GenerateAssemblyInfo>false</GenerateAssemblyInfo>
  </PropertyGroup>
</Project>
```

With the above configuration the `GenerateAssemblyInfo.AssemblyInfo.cs` will not be
generated.

## Assembly attributes mixed from two sources

Some assembly attributes cannot be defined with properties in csproj. So `AssemblyInfo.cs`
and auto-generated attributes can unite for completeness.

The csproj file below defines most of the available properties to auto-generate assembly
attributes into the `GenerateAssemblyInfo.AssemblyInfo.cs` file.

```xml
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Exe</OutputType>
    <TargetFramework>netcoreapp3.0</TargetFramework>
    <Company>Springfield Vegan</Company>
    <Product>Vegan Health</Product>
    <Authors>Elisabeth Simpson</Authors>
    <Copyright>Copyright (c) Springfield Vegan</Copyright>
    <AssemblyTitle>Health Control Application</AssemblyTitle>
    <Version>1.0.0.0</Version>
    <NeutralLanguage>en-US</NeutralLanguage>
  </PropertyGroup>
</Project>
```

The `AssemblyInfo.cs` file below defines the assembly attributes that have no corresponding
properties in the csproj.

```c#
using System.Reflection;
using System.Runtime.InteropServices;

// General information about the assembly is controlled through the following set of
// attributes, which complement the properties defined in the csproj file.
[assembly: AssemblyTrademark("D'oh!")]
[assembly: AssemblyCulture("")]

// Setting ComVisible to false makes the types in this assembly not visible
// to COM components.  If you need to access a type in this assembly from
// COM, set the ComVisible attribute to true on that type.
[assembly: ComVisible(false)]

// The following GUID is for the ID of the typelib if this project is exposed to COM
[assembly: Guid("7f941ba5-6732-41aa-80e1-505206c4e894")]
```

The above example shows how a Visual Studio project can define some assembly attributes
in the `AssemblyInfo.cs` file and auto-generate others from properties in the csproj file
into the `GenerateAssemblyInfo.AssemblyInfo.cs` file.

## See also

- [Additions to the csproj format for .NET Core](https://docs.microsoft.com/en-us/dotnet/core/tools/csproj)
- [MSBuild project file schema reference](https://docs.microsoft.com/en-us/visualstudio/msbuild/msbuild-project-file-schema-reference)
- [Microsoft.NET.GenerateAssemblyInfo.targets](https://github.com/dotnet/sdk/blob/master/src/Tasks/Microsoft.NET.Build.Tasks/targets/Microsoft.NET.GenerateAssemblyInfo.targets)

## License

This is free software. Redistribution and use in source and binary forms,
with or without modification, for any purpose and with or without fee are
hereby permitted. Altered source versions must be plainly marked as such.

If you find this software useful, an acknowledgment would be appreciated
but is not required.

THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OR CONDITION.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE TO ANYONE
FOR ANY DAMAGES RELATED TO THIS SOFTWARE, UNDER ANY KIND OF LEGAL CLAIM.
