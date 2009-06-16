using System;

namespace SolimarLicenseViewer
{
    public class VersionInfo
    {
        public static int MAJOR_REVISION_NUMBER = 3;
        public static int MINOR_REVISION_NUMBER = 0;
        public static int BUILD_NUMBER = 999999;
    }

    public class InstanceVersionInfo
    {
        public int major_revision_number = VersionInfo.MAJOR_REVISION_NUMBER;
        public int minor_revision_number = VersionInfo.MINOR_REVISION_NUMBER;
        public int build_number = VersionInfo.BUILD_NUMBER;
        public InstanceVersionInfo(int _major_revision_number, int _minor_revision_number, int _build_number)
        {
            major_revision_number = _major_revision_number;
            minor_revision_number = _minor_revision_number;
            build_number = _build_number;
        }
        override public string ToString()
        {
            return string.Format("{0:0}.{1:00}.{2:000000}", major_revision_number, minor_revision_number, build_number);
        }

        public bool IsEmpty()
        {
            return major_revision_number == 0 && minor_revision_number == 0 && build_number == 0;
        }

        //
        // Returns:
        //		< 0 : this is greater
        //		= 0 : this == _compareVersion
        //		> 0 : _compareVersion is greater
        //
        public int CompareTo(InstanceVersionInfo _compareVersion)
        {
            int retVal = major_revision_number - _compareVersion.major_revision_number;
            if (retVal == 0)
                retVal = minor_revision_number - _compareVersion.minor_revision_number;
            if (retVal == 0)
                retVal = build_number - _compareVersion.build_number;
            return retVal;
        }

    }
}
