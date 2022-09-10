// Solution for Roman to Integer in C# https://leetcode.com/problems/roman-to-integer/
// https://leetcode.com/submissions/detail/792813894/
// https://leetcode.com/submissions/detail/796017792/

public class Solution {
    
    public int RomanToInt(string s) {
        var RomanNumeralValues = new Dictionary <char, int> ();
        RomanNumeralValues.Add('I', 1);
        RomanNumeralValues.Add('V', 5);
        RomanNumeralValues.Add('X', 10);
        RomanNumeralValues.Add('L', 50);
        RomanNumeralValues.Add('C', 100);
        RomanNumeralValues.Add('D', 500);
        RomanNumeralValues.Add('M', 1000);
                
        int sum = 0;  
        int tmp = 0;
        int lastDigit = 1001;
        foreach (char character in s)
        {
            //Get the int value of each individual character
            int digit = RomanNumeralValues[character];
            
            // For cases of "II" and "XX" remember the value of them added together
            if (digit == lastDigit)
            {
                tmp += digit;
            }
            // For cases of "IV" and "XC" subtract the remembered value
            else if (digit > lastDigit)
            {
                tmp = digit - tmp;
            }
            // For cases of "VI" and "CX" add the remembered value
            else if (digit < lastDigit)
            {
                sum += tmp;
                tmp = digit;
            }

            lastDigit = digit;
        }
        sum += tmp;
        
        return sum;
    }
}