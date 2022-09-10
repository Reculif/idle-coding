# Solution for Two Sum in Python https://leetcode.com/problems/two-sum/
# https://leetcode.com/submissions/detail/796029238/

class Solution:
    def twoSum(self, nums: List[int], target: int) -> List[int]:
        TargetsDict = {}
        index = 0
                
        for i in nums:
            intToFind = target - i
            if intToFind in TargetsDict:
                return [TargetsDict[intToFind], index]
            TargetsDict.update({i: index})
            index += 1