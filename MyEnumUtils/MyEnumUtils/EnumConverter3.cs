using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace MyEnumUtils
{
    public static class EnumConverter3
    {
        public static string GetDescription3<T>(this T value) where T : struct
        {
            FieldInfo field;
            DescriptionAttribute attribute;
            string result;

            field = value.GetType().GetField(value.ToString());
            attribute = (DescriptionAttribute)Attribute.GetCustomAttribute(field, typeof(DescriptionAttribute));
            result = attribute != null ? attribute.Description : string.Empty;

            return result;
        }

        public static T GetValue3<T>(string value, T defaultValue)
        {
            T result;

            result = defaultValue;

            foreach (T id in Enum.GetValues(typeof(T)))
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
