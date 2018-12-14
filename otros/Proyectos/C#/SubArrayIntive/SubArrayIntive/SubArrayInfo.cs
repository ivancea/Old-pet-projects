using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SubArrayIntive
{
    public class SubArrayInfo
    {
        public int StartIndex;
        public int EndIndex;

        public SubArrayInfo(int startIndex, int endIndex)
        {
            StartIndex = startIndex;
            EndIndex = endIndex;
        }

        public override bool Equals(object obj)
        {
            SubArrayInfo info = obj as SubArrayInfo;

            return info != null
                && info.StartIndex == StartIndex
                && info.EndIndex == EndIndex;
        }
    }
}
