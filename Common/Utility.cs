//-----------------------------------------------------------------------
// <copyright file="Utility.cs" company="Master Arms">
//     Copyright (c) Master Arms. All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace MARS.Common
{
    using System;
    using System.IO;
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

        /// <summary>
        /// Reads a configuration value as a string from configuration file
        /// </summary>
        /// <param name="appName">Section in configuration file</param>
        /// <param name="keyName">Key name</param>
        /// <param name="defaultValue">Default value</param>
        /// <param name="fileName">Path to configuration file</param>
        /// <returns>The value read from the file if successful, otherwise the default value provided</returns>
        public static string ReadConfigurationString(string appName, string keyName, string defaultValue, string fileName)
        {
            const int SIZE = 255;
            var builder = new StringBuilder(SIZE);
            NativeFunctions.GetPrivateProfileString(appName, keyName, defaultValue, builder, SIZE, fileName);
            return builder.ToString();
        }

        /// <summary>
        /// Reads a configuration value as an integer from an configuration file
        /// </summary>
        /// <param name="appName">Section in configuration file</param>
        /// <param name="keyName">Key name</param>
        /// <param name="defaultValue">Default value</param>
        /// <param name="fileName">Path to configuration file</param>
        /// <returns>The value read from the file if successful, otherwise the default value provided</returns>
        public static int ReadConfigurationInteger(string appName, string keyName, int defaultValue, string fileName)
        {
            var temp = Utility.ReadConfigurationString(appName, keyName, defaultValue.ToString(), fileName);
            var value = defaultValue;

            try
            {
                value = Convert.ToInt32(temp);
            }
            catch (FormatException)
            {
            }

            return value;
        }

        /// <summary>
        /// Writes a configuration string to a configuration file
        /// </summary>
        /// <param name="appName">Section in configuration file</param>
        /// <param name="keyName">Key name</param>
        /// <param name="value">Value to set </param>
        /// <param name="fileName">Path to configuration file</param>
        public static void WriteConfigurationString(string appName, string keyName, string value, string fileName)
        {
            NativeFunctions.WritePrivateProfileString(appName, keyName, value, fileName);
        }

        /// <summary>
        /// Compares if two files are equal.
        /// </summary>
        /// <param name="firstFile">First file</param>
        /// <param name="secondFile">Second file</param>
        /// <returns>True if the files are equal, otherwise false</returns>
        public static bool FilesAreEqual(FileInfo firstFile, FileInfo secondFile)
        {
            if (!firstFile.Exists || !secondFile.Exists)
            {
                return false;
            }

            if (firstFile.Length != secondFile.Length)
            {
                return false;
            }

            using (var firstStream = firstFile.OpenRead())
            {
                using (var secondStream = secondFile.OpenRead())
                {
                    for (int i = 0; i < firstFile.Length; i++)
                    {
                        if (firstStream.ReadByte() != secondStream.ReadByte())
                        {
                            return false;
                        }
                    }
                }
            }

            return true;
        }
    }
}
