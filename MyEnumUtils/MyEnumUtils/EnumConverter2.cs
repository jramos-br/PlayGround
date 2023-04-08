using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace MyEnumUtils
{
    public static class EnumConverter2
    {
        public enum Unit
        {
            [Description("None")]
            None,

            [Description("cm")]
            Centimeter,

            [Description("mm")]
            Millimeter,

            [Description("pt")]
            Point,

            [Description("px")]
            Pixel,

            [Description("%")]
            Percent
        }

        public static string GetDescription2(this Unit value)
        {
            FieldInfo field;
            DescriptionAttribute attribute;
            string result;

            field = value.GetType().GetField(value.ToString());
            attribute = (DescriptionAttribute)Attribute.GetCustomAttribute(field, typeof(DescriptionAttribute));
            result = attribute != null ? attribute.Description : string.Empty;

            return result;
        }

        public static Unit GetValue2(string value)
        {
            Unit result;

            result = Unit.None;

            foreach (Unit id in Enum.GetValues(typeof(Unit)))
            {
                FieldInfo field;
                DescriptionAttribute attribute;

                field = id.GetType().GetField(id.ToString());
                attribute = Attribute.GetCustomAttribute(field, typeof(DescriptionAttribute)) as DescriptionAttribute;

                if (attribute != null && attribute.Description == value)
                {
                    result = id;
                    break;
                }
            }

            return result;
        }
    }
}
