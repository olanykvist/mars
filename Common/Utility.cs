//-----------------------------------------------------------------------
// <copyright file="Utility.cs" company="Master Arms">
//     Copyright (c) Master Arms. All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace MARS.Common
{
    using System;
    using System.Text;

    /// <summary>
    /// A utility class with some useful static methods
    /// </summary>
    public static class Utility
    {
        /// <summary>
        /// Gets a known/special folder path. A wrapper around the Win32 API SHGetKnownFolderPath function
        /// </summary>
        /// <param name="folderId">Id of folder</param>
        /// <returns>The path</returns>
        /// <seealso cref="KnownFolderId"></seealso>
        public static string GetKnownFolderPath(Guid folderId)
        {
            StringBuilder builder = null;
            NativeFunctions.SHGetKnownFolderPath(ref folderId, 0, IntPtr.Zero, out builder);
            return builder.ToString();
        }

        public static string ReadConfigurationString(string appName, string keyName, string defaultValue, string fileName)
        {
            const int SIZE = 255;
            StringBuilder builder = new StringBuilder(SIZE);
            NativeFunctions.GetPrivateProfileString(appName, keyName, defaultValue, builder, SIZE, fileName);
            return builder.ToString();
        }

        public static void WriteConfigurationString(string appName, string keyName, string value, string fileName)
        {
            NativeFunctions.WritePrivateProfileString(appName, keyName, value, fileName);
        }
    }
}
