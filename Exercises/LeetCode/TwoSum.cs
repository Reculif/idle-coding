// Solution for Two Sum in C# https://leetcode.com/problems/two-sum/
// https://leetcode.com/submissions/detail/794537567/

public class Solution {
    public int[] TwoSum(int[] nums, int target) {    
        for (int i = 0; i < nums.Length-1; i++)
        {
             for (int j = i+1; j < nums.Length; j++)  
             {
                 if (nums[i] + nums[j] == target)
                 {
                     return new int[]{i, j};
                 }
             }
        }
        return new int[]{0,0};
    }
}