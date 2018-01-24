template <class T>
void breadth_first(TreeNode<T> *p) {
  std::vector<TreeNode<T>*> current;
  std::vector<TreeNode<T>*> next;
  
  // handle an empty tree
  if (p != NULL) {
    current.push_back(p);
  }

  // loop over all levels of the tree
  while (current.size() > 0) {
    // loop over all elements on this level
    for (int x = 0; x < current.size(); x++) {
      std::cout << " " << current[x]->value;  // the "do something" part of this traversal
      if (current[x]->left != NULL)
        next.push_back(current[x]->left);
      if (current[x]->right != NULL)
        next.push_back(current[x]->right);
    }
    // switch to the next level, empty next vector to receive following level
    current = next;
    next.clear();
  }
}
