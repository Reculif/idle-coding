# Solution for Valid Parentheses https://leetcode.com/problems/valid-parentheses/
# https://leetcode.com/submissions/detail/801737868/

class Solution:
    def isValid(self, s: str) -> bool:
        stack = []
        opening = {'(':')',
                   '[':']',
                   '{':'}'}
        
        if (len(s) % 2) is not 0:
            return False
        
        for character in s:
            if character in opening.keys():
                stack.append(character)
            elif not stack:
                return False
            elif character == opening[stack[-1]]:
                stack.pop()
            else:
                return False
        if stack:
            return False
        else:
            return True