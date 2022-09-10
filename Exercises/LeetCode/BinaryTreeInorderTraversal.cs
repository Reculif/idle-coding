// Solution for Binary Tree Inorder Traversal in C# https://leetcode.com/problems/binary-tree-inorder-traversal/
// https://leetcode.com/submissions/detail/794573886/

public class Solution {
    public IList<int> InorderTraversal(TreeNode root) {
        List<int> returnList = new List<int>();
        if (root != null)
        {
            if (root.left != null)
            {
                returnList.AddRange(InorderTraversal(root.left));
            }
            returnList.Add(root.val);
            if (root.right != null)
            {
                returnList.AddRange(InorderTraversal(root.right));
            }
        }
        return returnList;
    }
}