static int resize_tree(struct bst *tree, size_t new_capacity)
{
    struct value **new_data = realloc(tree->data, new_capacity * sizeof(struct value *));
    if (!new_data)
        return 0;
    
    for (size_t i = tree->capacity; i < new_capacity; i++)
        new_data[i] = NULL;
    
    tree->data = new_data;
    tree->capacity = new_capacity;
    return 1;
}



void add(struct bst *tree, int value)
{
    if (!tree)
        return;

    if (tree->size >= tree->capacity)
    {
        if (!resize_tree(tree, tree->capacity * 2))
            return;
    }

    size_t i = 0;
    while (i < tree->capacity && tree->data[i] != NULL)
    {
        if (value == tree->data[i]->val)
            return; 

        if (value < tree->data[i]->val)
            i = 2 * i + 1; 
        else
            i = 2 * i + 2; 
    }

    if (i >= tree->capacity)
    {
        if (!resize_tree(tree, i + 1))
            return;
    }

    struct value *new_val = malloc(sizeof(struct value));
    if (!new_val)
        return;

    new_val->val = value;
    tree->data[i] = new_val;
    tree->size++;
}
