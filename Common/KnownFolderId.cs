//-----------------------------------------------------------------------
// <copyright file="KnownFolderId.cs" company="Master Arms">
//     Copyright (c) Master Arms. All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace MARS.Common
{
    using System;

    /// <summary>
    /// This class contains Guids that identify standard folders registered with the system
    /// </summary>
    public static class KnownFolderId
    {
        /// <summary>
        /// Default path: %USERPROFILE%\Saved Games
        /// </summary>
        public static readonly Guid SavedGames = new Guid("{4C5C32FF-BB9D-43b0-B5B4-2D72E54EAAA4}");

        /// <summary>
        /// Default path: %LOCALAPPDATA% (%USERPROFILE%\AppData\Local)
        /// </summary>
        public static readonly Guid LocalAppData = new Guid("{F1B32785-6FBA-4FCF-9D55-7B8E7F157091}");
    }
}
