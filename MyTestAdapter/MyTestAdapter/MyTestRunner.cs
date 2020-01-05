// A custom adapter to discover and execute MSTest Framework based tests.
//
// Copyright (c) 2019 Jorge Ramos (mailto jramos at pobox dot com)
//
// This is free software. Redistribution and use in source and binary forms,
// with or without modification, for any purpose and with or without fee are
// hereby permitted. Altered source versions must be plainly marked as such.
//
// If you find this software useful, an acknowledgment would be appreciated
// but is not required.
//
// THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT ANY WARRANTY OR CONDITION.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE TO ANYONE
// FOR ANY DAMAGES RELATED TO THIS SOFTWARE, UNDER ANY KIND OF LEGAL CLAIM.

using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace MyTestAdapter
{
    /// <summary>
    /// A Test Runner for MSTest Framework.
    /// </summary>
    /// <remarks>
    /// This is a simple Runner different from MSTest Runner in the following ways:
    /// - You have to give the assembly to be tested.
    /// - All tests are run.
    /// - Test output is not captured.
    /// - TestCleanup, ClassCleanup and AssemblyCleanup are always called.
    /// - ClassCleanup is called after the tests of each class.
    /// </remarks>
    public class MyTestRunner
    {
        static bool HasTestClassAttribute(Type t) => t.GetCustomAttribute<TestClassAttribute>() != null;
        static bool HasTestMethodAttribute(MethodInfo m) => m.GetCustomAttribute<TestMethodAttribute>() != null;
        static bool HasDataTestMethodAttribute(MethodInfo m) => m.GetCustomAttribute<DataTestMethodAttribute>() != null;
        static bool HasAssemblyInitializeAttribute(MethodInfo m) => m.GetCustomAttribute<AssemblyInitializeAttribute>() != null;
        static bool HasAssemblyCleanupAttribute(MethodInfo m) => m.GetCustomAttribute<AssemblyCleanupAttribute>() != null;
        static bool HasClassInitializeAttribute(MethodInfo m) => m.GetCustomAttribute<ClassInitializeAttribute>() != null;
        static bool HasClassCleanupAttribute(MethodInfo m) => m.GetCustomAttribute<ClassCleanupAttribute>() != null;
        static bool HasTestInitializeAttribute(MethodInfo m) => m.GetCustomAttribute<TestInitializeAttribute>() != null;
        static bool HasTestCleanupAttribute(MethodInfo m) => m.GetCustomAttribute<TestCleanupAttribute>() != null;

        // https://stackoverflow.com/questions/1175888/determine-if-a-type-is-static
        static bool IsStatic(Type t) => t.IsAbstract && t.IsSealed && !t.IsInterface;

        static bool IsTestClass(Type t) => t.IsPublic && !IsStatic(t) && HasTestClassAttribute(t);
        static bool IsTestMethod(MethodInfo m) => m.IsPublic && !m.IsStatic && HasTestMethodAttribute(m) && IsTestClass(m.DeclaringType);
        static bool IsDataTestMethod(MethodInfo m) => m.IsPublic && !m.IsStatic && HasDataTestMethodAttribute(m) && IsTestClass(m.DeclaringType);
        static bool IsAssemblyInitialize(MethodInfo m) => m.IsPublic && m.IsStatic && HasAssemblyInitializeAttribute(m) && IsTestClass(m.DeclaringType);
        static bool IsAssemblyCleanup(MethodInfo m) => m.IsPublic && m.IsStatic && HasAssemblyCleanupAttribute(m) && IsTestClass(m.DeclaringType);
        static bool IsClassInitialize(MethodInfo m) => m.IsPublic && m.IsStatic && HasClassInitializeAttribute(m) && IsTestClass(m.DeclaringType);
        static bool IsClassCleanup(MethodInfo m) => m.IsPublic && m.IsStatic && HasClassCleanupAttribute(m) && IsTestClass(m.DeclaringType);
        static bool IsTestInitialize(MethodInfo m) => m.IsPublic && !m.IsStatic && HasTestInitializeAttribute(m) && IsTestClass(m.DeclaringType);
        static bool IsTestCleanup(MethodInfo m) => m.IsPublic && !m.IsStatic && HasTestCleanupAttribute(m) && IsTestClass(m.DeclaringType);

        static void PrintException(Exception ex, MyTestContext context)
        {
            if (ex is TargetInvocationException)
            {
                var ie = ex.InnerException;

                if (ie is AssertFailedException || ie is InternalTestFailureException)
                {
                    Trace.WriteLine(string.Format("{0}: {1}", ie.GetType().Name, ie.Message));
                    Trace.WriteLine(ie.StackTrace);

                    if (context != null)
                    {
                        context.SetLastException(ie);
                        context.SetCurrentTestOutcome(UnitTestOutcome.Failed);
                    }

                    Environment.ExitCode = 1;
                }
                else if (ie is AssertInconclusiveException)
                {
                    Trace.WriteLine(string.Format("{0}: {1}", ie.GetType().Name, ie.Message));
                    Trace.WriteLine(ie.StackTrace);

                    if (context != null)
                    {
                        context.SetLastException(ie);
                        context.SetCurrentTestOutcome(UnitTestOutcome.Inconclusive);
                    }

                    Environment.ExitCode = 1;
                }
                else if (ie != null)
                {
                    Trace.WriteLine(string.Format("{0}: {1}", ie.GetType().Name, ie.Message));
                    Trace.WriteLine(ie.StackTrace);

                    if (context != null)
                    {
                        context.SetLastException(ie);
                        context.SetCurrentTestOutcome(UnitTestOutcome.Error);
                    }

                    Environment.ExitCode = 3;
                }
                else
                {
                    Trace.WriteLine(string.Format("{0}: {1}", ex.GetType().Name, ex.Message));
                    Trace.WriteLine(ex.StackTrace);

                    if (context != null)
                    {
                        context.SetLastException(ex);
                        context.SetCurrentTestOutcome(UnitTestOutcome.Error);
                    }

                    Environment.ExitCode = 3;
                }
            }
            else
            {
                Trace.WriteLine(string.Format("{0}: {1}", ex.GetType().Name, ex.Message));
                Trace.WriteLine(ex.StackTrace);

                if (context != null)
                {
                    context.SetLastException(ex);
                    context.SetCurrentTestOutcome(UnitTestOutcome.Error);
                }

                Environment.ExitCode = 3;
            }
        }

        static bool IsSameParametersLength(ParameterInfo[] parameterInfo, object[] parameters)
        {
            if ((parameterInfo == null || parameterInfo.Length == 0) && (parameters == null || parameters.Length == 0))
            {
                return true;
            }

            if (parameterInfo != null && parameters != null && parameterInfo.Length == parameters.Length)
            {
                return true;
            }

            return false;
        }

        static void Invoke(MethodInfo methodInfo, object classInstance, object[] parameters)
        {
            if (methodInfo == null)
            {
                return;
            }

            if (!IsSameParametersLength(methodInfo.GetParameters(), parameters))
            {
                throw new TargetParameterCountException();
            }

            var isAwaitable = methodInfo.ReturnType.GetMethod(nameof(Task.GetAwaiter)) != null;

            if (isAwaitable)
            {
                var task = methodInfo.Invoke(classInstance, parameters) as Task;
                task?.GetAwaiter().GetResult();
            }
            else
            {
                methodInfo.Invoke(classInstance, parameters);
            }
        }

        static bool Invoke(MyTestContext context, MethodInfo methodInfo, object classInstance, object[] parameters)
        {
            bool result = true;

            try
            {
                Invoke(methodInfo, classInstance, parameters);
            }
            catch (Exception ex)
            {
                PrintException(ex, context);
                result = false;
            }

            return result;
        }

        /// <summary>
        /// Test instance data.
        /// </summary>
        class TestInstance
        {
            public string DisplayName;
            public object[] Parameters;
        }

        static List<TestInstance> CreateTestInstanceList(MethodInfo methodInfo)
        {
            var testInstances = new List<TestInstance>();

            if (IsDataTestMethod(methodInfo))
            {
                var dynamicDataAttribute = methodInfo.GetCustomAttribute<DynamicDataAttribute>();

                if (dynamicDataAttribute != null)
                {
                    var dynamicDataSet = dynamicDataAttribute.GetData(methodInfo);

                    foreach (var dynamicData in dynamicDataSet)
                    {
                        var testInstance = new TestInstance()
                        {
                            DisplayName = dynamicDataAttribute.GetDisplayName(methodInfo, dynamicData),
                            Parameters = dynamicData
                        };

                        testInstances.Add(testInstance);
                    }
                }

                var dataRowAttributes = methodInfo.GetCustomAttributes<DataRowAttribute>();

                foreach (var dataRowAttribute in dataRowAttributes)
                {
                    var testInstance = new TestInstance()
                    {
                        DisplayName = dataRowAttribute.GetDisplayName(methodInfo, dataRowAttribute.Data),
                        Parameters = dataRowAttribute.Data
                    };

                    testInstances.Add(testInstance);
                }
            }

            if (testInstances.Count == 0)
            {
                var testInstance = new TestInstance()
                {
                    DisplayName = methodInfo.Name,
                    Parameters = null
                };

                testInstances.Add(testInstance);
            }

            return testInstances;
        }

        static object CreateTestClassInstance(MyTestContext context, Type type)
        {
            object testClassInstance;

            try
            {
                var testContextProperty = type.GetProperty("TestContext");
                testClassInstance = Activator.CreateInstance(type, null);
                testContextProperty?.SetValue(testClassInstance, context);
            }
            catch (Exception ex)
            {
                PrintException(ex, context);
                testClassInstance = null;
            }

            return testClassInstance;
        }

        /// <summary>
        /// The test results list.
        /// </summary>
        List<MyTestResult> TestResults { get; } = new List<MyTestResult>();

        MyTestOutcome GetTestOutcome(MyTestContext context)
        {
            switch (context.CurrentTestOutcome)
            {
                case UnitTestOutcome.Unknown:
                    return MyTestOutcome.Unknown;
                case UnitTestOutcome.InProgress:
                    return MyTestOutcome.InProgress;
                case UnitTestOutcome.Inconclusive:
                    return MyTestOutcome.Inconclusive;
                case UnitTestOutcome.Failed:
                    return MyTestOutcome.Failed;
                case UnitTestOutcome.Passed:
                    return MyTestOutcome.Passed;
            }

            return MyTestOutcome.Error;
        }

        void AddTestResult(MyTestContext context, object[] parameters, double elapsedSeconds)
        {
            var testResult = new MyTestResult()
            {
                TestName = context.TestName,
                DisplayName = context.TestDisplayName,
                Parameters = parameters,
                TestOutcome = GetTestOutcome(context),
                LastException = context.LastException,
                ElapsedSeconds = elapsedSeconds
            };

            TestResults.Add(testResult);
        }

        /// <summary>
        /// Executes the tests found in the assembly.
        /// </summary>
        /// <param name="assembly">A loaded assembly.</param>
        void Run(Assembly assembly)
        {
            var types = assembly.GetTypes().Where(t => IsTestClass(t)).ToList();
            var methods = types.SelectMany(t => t.GetMethods()).ToList();
            var assemblyInitialize = methods.SingleOrDefault(m => IsAssemblyInitialize(m));
            var assemblyInitializePending = true;
            var assemblyInitializeError = false;
            var assemblyCleanup = methods.SingleOrDefault(m => IsAssemblyCleanup(m));
            var assemblyCleanupPending = false;

            foreach (var type in types)
            {
                var classMethods = methods.Where(m => m.DeclaringType == type).ToList();
                var classInitialize = classMethods.SingleOrDefault(m => IsClassInitialize(m));
                var classInitializePending = true;
                var classInitializeError = false;
                var classCleanup = classMethods.SingleOrDefault(m => IsClassCleanup(m));
                var classCleanupPending = false;
                var testInitializeMethod = classMethods.SingleOrDefault(m => IsTestInitialize(m));
                var testCleanupMethod = classMethods.SingleOrDefault(m => IsTestCleanup(m));
                var testMethods = classMethods.Where(m => IsTestMethod(m)); // || IsDataTestMethod not needed

                foreach (var testMethod in testMethods)
                {
                    var testInstances = CreateTestInstanceList(testMethod);

                    foreach (var testInstance in testInstances)
                    {
                        var context = new MyTestContext();
                        var contextParam = new object[] { context };

                        context.SetFullyQualifiedTestClassName(type.FullName);
                        context.SetTestName(testMethod.Name);
                        context.SetTestDisplayName(testInstance.DisplayName);
                        context.SetCurrentTestOutcome(UnitTestOutcome.InProgress);

                        if (assemblyInitializePending)
                        {
                            assemblyInitializeError = !Invoke(context, assemblyInitialize, null, contextParam);
                            assemblyCleanupPending = true;
                            assemblyInitializePending = false;

                            if (assemblyInitializeError)
                            {
                                break;
                            }
                        }

                        if (classInitializePending)
                        {
                            classInitializeError = !Invoke(context, classInitialize, null, contextParam);
                            classCleanupPending = true;
                            classInitializePending = false;

                            if (classInitializeError)
                            {
                                break;
                            }
                        }

                        var testClassInstance = CreateTestClassInstance(context, type);

                        if (testClassInstance != null)
                        {
                            var stopwatch = Stopwatch.StartNew();

                            if (Invoke(context, testInitializeMethod, testClassInstance, null))
                            {
                                if (Invoke(context, testMethod, testClassInstance, testInstance.Parameters))
                                {
                                    context.SetCurrentTestOutcome(UnitTestOutcome.Passed);
                                }
                            }

                            Invoke(context, testCleanupMethod, testClassInstance, null);

                            double elapsedSeconds = (double)stopwatch.ElapsedTicks / Stopwatch.Frequency;

                            AddTestResult(context, testInstance.Parameters, elapsedSeconds);
                        }
                    }

                    if (assemblyInitializeError)
                    {
                        break;
                    }

                    if (classInitializeError)
                    {
                        break;
                    }
                }

                if (classCleanupPending)
                {
                    Invoke(null, classCleanup, null, null);
                }

                if (assemblyInitializeError)
                {
                    break;
                }
            }

            if (assemblyCleanupPending)
            {
                Invoke(null, assemblyCleanup, null, null);
            }
        }

        /// <summary>
        /// Executes the tests found in the assembly.
        /// </summary>
        /// <param name="assembly">A loaded assembly.</param>
        /// <returns>The test results.</returns>
        public static List<MyTestResult> RunTests(Assembly assembly)
        {
            List<MyTestResult> results;

            try
            {
                var runner = new MyTestRunner();
                runner.Run(assembly);
                results = runner.TestResults;
            }
            catch (Exception ex)
            {
                PrintException(ex, null);
                results = null;
            }

            return results;
        }

        /// <summary>
        /// Executes the tests found in the assembly.
        /// </summary>
        /// <param name="assemblyFile">The name of the assembly file (normally a DLL).</param>
        /// <returns>The test results.</returns>
        public static List<MyTestResult> RunTests(string assemblyFile)
        {
            List<MyTestResult> results;

            try
            {
                var assembly = Assembly.LoadFrom(assemblyFile);
                results = RunTests(assembly);
            }
            catch (Exception ex)
            {
                PrintException(ex, null);
                results = null;
            }

            return results;
        }
    }
}
