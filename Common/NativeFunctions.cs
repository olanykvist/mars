//-----------------------------------------------------------------------
// <copyright file="NativeFunctions.cs" company="Master Arms">
//     Copyright (c) Master Arms. All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace MARS.Common
{
    using System;
    using System.Runtime.InteropServices;
    using System.Text;

    /// <summary>
    /// This class contains external Win32 API functions
    /// </summary>
    public static class NativeFunctions
    {
        [DllImport("shell32.dll", CharSet = CharSet.Auto)]
        internal static extern uint SHGetKnownFolderPath(ref Guid id, int flags, IntPtr token, out StringBuilder path);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        internal static extern uint GetPrivateProfileString(string appName, string keyName, string defaultValue, StringBuilder returnedString, uint size, string fileName);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        internal static extern bool WritePrivateProfileString(string appName, string keyName, string value, string fileName);
    }
}