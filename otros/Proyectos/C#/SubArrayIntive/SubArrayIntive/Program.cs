using System;

namespace SubArrayIntive
{
    public class Program
    {
        /// <summary>
        /// Returns the smallest sub-array that, when sorted, makes the entire array to be sorted.
        /// </summary>
        /// <param name="array">Array to examine</param>
        /// <returns>Null if the array is sorted. Else, a SubArrayInfo with the sub-array indexes [start, end)</returns>
        public static SubArrayInfo getSubArray(int[] array)
        {
            SubArrayInfo result = null;

            // Searching for first unsorted element
            for (int i = 0; i < array.Length - 1; i++)
            {
                if (array[i] > array[i + 1])
                {
                    result = new SubArrayInfo(i, i + 2);

                    break;
                }
            }

            // Array is sorted or empty
            if (result == null)
            {
                return null;
            }

            // Searching for last unsorted element
            for (int i = array.Length - 2; i > result.StartIndex; i--)
            {
                if (array[i] > array[i + 1])
                {
                    result.EndIndex = i + 2;

                    break;
                }
            }

            // Serach for min and max values
            /* Less efficient
            var subArray = array.Skip(result.StartIndex).Take(result.EndIndex - result.StartIndex);

            int min = subArray.Min(),
                max = subArray.Max();
            */

            int min = array[result.StartIndex],
                max = array[result.StartIndex];

            for (int i = result.StartIndex + 1; i < result.EndIndex; i++)
            {
                if (array[i] < min)
                {
                    min = array[i];
                }
                    
                if (array[i] > max)
                {
                    max = array[i];
                }
            }

            // Searching for elements before the sub-array that are greater than min
            int newIndex = result.StartIndex;

            for (int i = 0; i < result.StartIndex; i++)
            {
                if (array[i] > min)
                {
                    newIndex = i;

                    break;
                }
            }

            result.StartIndex = newIndex;
            
            // Searching for elements after the sub-array that are lesser than max
            newIndex = result.EndIndex;

            for (int i = array.Length - 1; i >= result.EndIndex; i--)
            {
                if (array[i] < max)
                {
                    newIndex = i + 1;

                    break;
                }
            }

            result.EndIndex = newIndex;
            
            return result;
        }

        static void Main(string[] args)
        {
            if (args.Length < 2)
            {
                Console.WriteLine("Usage: <program> <integer list>");
                Console.WriteLine("Example: SubArrayintive 5 6 9 8 9 10");

                return;
            }

            int[] array = new int[args.Length - 1];

            // Load integers
            for (int i = 1; i < args.Length; i++)
            {
                if (!int.TryParse(args[i], out array[i - 1]))
                {
                    Console.WriteLine("Invalid argument (" + (i - 1) + ")");

                    return;
                }
            }
            
            SubArrayInfo result = getSubArray(array);

            if (result == null)
            {
                Console.WriteLine("It is already sorted");
            }
            else
            {
                Console.WriteLine(result.StartIndex + " to " + result.EndIndex);

                Console.Write("{ ");

                for (int i = result.StartIndex; i < result.EndIndex; i++)
                {
                    Console.Write(array[i] + (i + 1 < result.EndIndex ? ", " : ""));
                }

                Console.WriteLine(" }");
            }
        }
    }
}
