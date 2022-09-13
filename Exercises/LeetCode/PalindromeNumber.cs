// Solution for Two Sum in C# https://leetcode.com/problems/palindrome-number/submissions/
// https://leetcode.com/submissions/detail/798423756/

public class Solution {
    public bool IsPalindrome(int x) {
        if (x < 0)
            return false;
        
        int forward = x;
        int reverse = 0;
        
        while (forward >= 10)
        {
            reverse *= 10;
            reverse += forward % 10;
            forward /= 10;
        }
        
        return reverse == x / 10 && forward == x % 10;
    }
}