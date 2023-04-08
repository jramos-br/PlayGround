using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace MyEnumUtils
{
    public static class EnumConverter4
    {
        public static string GetDescription4<T>(this T value) where T : struct
        {
            FieldInfo field;
            DescriptionAttribute attribute;
            string result;

            field = typeof(T).GetField(value.ToString());
            attribute = (DescriptionAttribute)Attribute.GetCustomAttribute(field, typeof(DescriptionAttribute));
            result = attribute != null ? attribute.Description : string.Empty;

            return result;
        }

        public static T GetValue4<T>(string value, T defaultValue)
        {
            T result;

            result = defaultValue;

            foreach (T id in Enum.GetValues(typeof(T)))
            {
                FieldInfo field;
                DescriptionAttribute attribute;

                field = typeof(T).GetField(id.ToString());
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
