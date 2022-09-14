# Solution for Longest Common Prefix in Python https://leetcode.com/problems/longest-common-prefix/
# https://leetcode.com/submissions/detail/799563300/

class Solution:
    def longestCommonPrefix(self, strs: List[str]) -> str:       
        prefix = ""
        character = ""
        match = True
        i = 0
        while(match):
            if len(strs[0]) is 0:
                return prefix
            prefix += character
            character = strs[0][i]
            for string in strs:
                if len(string) is 0:
                    return prefix
                if string[i] is not character:
                    return prefix
                if len(string)-1 is i:
                    match = False
            i += 1
        prefix += character
        return prefix