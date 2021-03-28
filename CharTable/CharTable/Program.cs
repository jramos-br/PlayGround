using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CharTable
{
    class Program
    {
        // The Unicode Standard sets aside 66 noncharacter code points. The last two code points of
        // each plane are noncharacters: U+FFFE and U+FFFF on the BMP, U+1FFFE and U+1FFFF
        // on Plane 1, and so on, up to U+10FFFE and U+10FFFF on Plane 16, for a total of 34 code
        // points. In addition, there is a contiguous range of another 32 noncharacter code points in
        // the BMP: U+FDD0..U+FDEF. For historical reasons, the range U+FDD0..U+FDEF is contained
        // within the Arabic Presentation Forms-A block, but those noncharacters are not
        // “Arabic noncharacters” or “right-to-left noncharacters,” and are not distinguished in any
        // other way from the other noncharacters, except in their code point values.
        // http://www.unicode.org/versions/Unicode10.0.0/UnicodeStandard-10.0.pdf
        public static bool IsNonCharacter(int codePoint)
        {
            return (codePoint < 0 || codePoint > 0x10FFFF) ||
                ((codePoint & 0xFFFF) == 0xFFFE || (codePoint & 0xFFFF) == 0xFFFF) ||
                (codePoint >= 0xFDD0 && codePoint <= 0xFDEF);
        }

        static void Main(string[] args)
        {
            int codePoint = 0;

            while (true)
            {
                if (!IsNonCharacter(codePoint))
                {
                    char ch = (char)codePoint;
                    UnicodeCategory unicodeCategory = CharUnicodeInfo.GetUnicodeCategory(ch);
                    if (unicodeCategory == UnicodeCategory.Control)
                    {
                        // Console.Out.WriteLine("codePoint={0} unicodeCategory={1}", codePoint, unicodeCategory);
                    }
                    else
                    {
                        // Console.Out.WriteLine("codePoint={0} unicodeCategory={1} char={2}", codePoint, unicodeCategory, ch);
                        Console.Out.Write(ch);
                    }
                }

                if (codePoint == 255)
                {
                    break;
                }

                ++codePoint;
            }
        }
    }
}
