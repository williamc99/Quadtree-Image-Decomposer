/**
 * CSC A48 - Intro to Computer Science II, Summer 2020
 * 
 * This is the program file where you will implement your solution for
 * assignment 2. Please make sure you read through this file carefully
 * and that you understand what is provided and what you need to complete.
 * 
 * You will need to have read the handout carefully. 
 * 
 * Parts where you have to implement functionality are clearly labeled TODO
 * 
 * Be sure to test your work thoroughly, our testing will be extensive
 * and will check that your solution is *correct*, not only that it
 * provides functionality.
 * 
 * (c) 2020 Mustafa Quraish 
 */

#include "imgUtils.c"

/**
 * This is the structure we are going to use to store each individual node of 
 * the BST. Remember that each Quad corresponds to a rectangular area on the 
 * image:
 *
 *                (tx,ty)         w
 *                   x-------------------------
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                 h |          Quad          |
 *                   |    key = tx+(ty*sx)    |
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                   -------------------------x
 *                                       (tx + w, ty + h)
 *
 */
typedef struct quad {
  int tx, ty;  // The (x,y) coordinates of the top-left pixel in the quad
  int w;       // How many pixels wide the quad is
  int h;       // How many pixels high the quad is

  int sx;  // Width of the original image, this is needed for the key.
           // This *MUST* be the same for all nodes in the BST

  int key;  // A unique identifier (remember we discussed BST nodes
            // should have unique keys to identify each node). The
            // key identifier here will be created as:
            //       key = tx + (ty * sx)
            // This means that only one quad can start at a specific
            // pixel.

  int wsplit;  // 1 if this quad is supposed to be split along the width
               // 0 if this quad is supposed to be split along the height

  /** 
   * TODO: Complete the definition of the Quad struct
   */

  struct quad *left; //Keys that are smaller than root go on left side
  struct quad *right; //Keys that are larger than root go on the right side

} Quad;

///////////////////////////////////////////////////////////////////////////////

Quad *new_Quad(int tx, int ty, int w, int h, int wsplit, int sx) {
  /**
   * This function creates and initializes a new Quad for a rectangle starting 
   * at (tx, ty) with a width 'w' and height 'h'. The width of the image in 
   * which this rectangle exists is 'sx', use this to compute the key as:
   *
   * 		key = tx + (ty * sx)
   * 
   * TODO: Implement this function
   */

  Quad *new_quad = (Quad*)calloc(1, sizeof(Quad));
  new_quad->tx = tx;
  new_quad->ty = ty;
  new_quad->w = w;
  new_quad->h = h;
  new_quad->wsplit = wsplit;
  new_quad->sx = sx;
  new_quad->key = tx + (ty * sx);
  new_quad->left = NULL;
  new_quad->right = NULL;

  return new_quad;
}

///////////////////////////////////////////////////////////////////////////////

int BST_traverse(Quad *root, int value){
  
  if (root != NULL){
    if (root->key == value){
      return 1;    
    }
    BST_traverse(root->left, value);
    BST_traverse(root->right, value);
  }
  return 0;
}

Quad *BST_insert(Quad *root, Quad *new_node) {
  /**
   * This function inserts a new Quad node into the BST rooted at 'root'. The 
   * new_node must already be initialized with valid data, and must have a 
   * unique key.
   *
   * Your function must make sure that there are no duplicate nodes with the 
   * same key in the BST, and if it finds any you should print the following 
   * message to the screen:
   *
   * printf("Duplicate Quad (tx,ty,sx)=%d,%d, %d, was ignored\n",....); 
   * (of course you need to provide the relevant variables to print)
   *
   * And it must return without inserting anything in the BST.
   *
   * TODO: Implement this function
   */

  int check = 0; //0 means no match, 1 means match

  check = BST_traverse(root, new_node->key);

  if (check == 1){
    printf("Duplicate Quad (tx,ty,sx)=%d,%d, %d, was ignored\n", new_node->tx, new_node->ty, new_node->sx);
    return root; 
  }

  if (root == NULL){
    return new_node;
  }
  else{
    if (root->key < new_node->key){
      root->right = BST_insert(root->right, new_node);
    }
    else{
      root->left = BST_insert(root->left, new_node);
    }
  }
  return root;
}

///////////////////////////////////////////////////////////////////////////////

Quad *BST_search(Quad *root, int tx, int ty) {
  /**
   * This function searches the BST for a Quad at the speficied position. If 
   * found, it must return a pointer to the quad that contains it.
   *
   * Search has to happen according to the BST search process - so you need to 
   * figure out what value to use during the search process to decide which 
   * branch of the tree to search next.
   *
   * Note that the 'sx' value does not need to be passed in here since it must 
   * be the same as the one in any Quad already in the tree.
   *
   * Return NULL if the Quad doesn't exist in the BST.
   *
   * TODO: Implement this function
   */

  if (root == NULL){
    return NULL;
  }
  if (root->tx == tx && root->ty == ty){
    return root;
  }
  if (root->key < tx + (ty*root->sx)){
    return BST_search(root->right, tx, ty);
  }
  else{
    return BST_search(root->left, tx, ty);
  }

  return NULL;
}

///////////////////////////////////////////////////////////////////////////////

Quad *find_successor(Quad *right_child) {
  /**
   * This function finds the successor of a Quad node by searching the right 
   * subtree for the node that is most to the left (that will be the node
   * with the smallest key in that subtree)
   *
   * TODO: Implement this function
   */

  if (right_child->left == NULL){
    return right_child;
  }
  else{
    return find_successor(right_child->left);
  }
  
}

///////////////////////////////////////////////////////////////////////////////

Quad *BST_delete(Quad *root, int tx, int ty) {
  /**
   * Deletes from the BST a Quad at the specified position. You must implement 
   * the three cases of BST deletion we discussed in class. Make sure the 
   * function can remove a Quad at any position without breaking the tree!
   *
   * Once again, remember that 'sx' is stored in the tree.
   *
   * TODO: Implement this function
   */


  int key = 0;
  key = tx + (ty*root->sx);

  Quad *new_root = root;
  Quad *successor = NULL;

  if (root != NULL){
    if (root->key < key){
      root->right = BST_delete(root->right, tx, ty);
    }
    else if (root->key > key){
      root->left = BST_delete(root->left, tx, ty);
    }
    else{
      if (root->left == NULL){
        new_root = root->right;
        free(root);
      }
      else if (root->right == NULL){
        new_root = root->left;
        free(root);
      }
      else{
        successor = find_successor(root->right);
        root->key = successor->key;
        root->h = successor->h;
        root->tx = successor->tx;
        root->ty = successor->ty;
        root->w = successor->w;
        root->wsplit = successor->wsplit;
        root->right = BST_delete(root->right, root->tx, root->ty);
      }
    }
  }

  return new_root; 
}

///////////////////////////////////////////////////////////////////////////////

Quad *delete_BST(Quad *root) {
  /**
   * This function deletes the BST and frees all memory used for nodes in it. 
   * Recall that there is a specific order in which this needs to be done! 
   * (consult the Unit 4 notes as needed)
   * 
   * This function should return NULL.
   *
   * TODO: Implement this function
   */

  if (root != NULL){
    delete_BST(root->left);
    delete_BST(root->right);
    free(root);
  }
  
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////

Quad* BST_inorder(Quad *root, int depth) {
  /**
   * This function performs an in-order traversal of the BST and prints out the
   * information for each Quad using this exactly this print statement:
   *
   *  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",...)
   *
   * Obviously, you must provide the variables to the printf function - we're 
   * just giving you the formatting string.
   *
   * The depth value is increased by 1 for each recursive call so when you 
   * print, you can see at what level each node is located! (this should help 
   * you debug your code by making it easier to check the shape of your BST).
   *
   * TODO: Implement this function
   */

  if (root != NULL){
    //depth++;
    root->left = BST_inorder(root->left, depth+1);
    printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n", depth, root->key, root->tx, root->ty, root->w, root->h, root->wsplit);
    root->right = BST_inorder(root->right, depth+1);
  }

  return root;
}

///////////////////////////////////////////////////////////////////////////////

void BST_preorder(Quad *root, int depth) {
  /**
   * This function performs a pre-order traversal of the BST and prints out the 
   * information for each Quad using this exactly this print statement:
   *
   *  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",...)
   *
   * Obviously, you must provide the variables to the printf function - we're 
   * just giving you the formatting string.
   *
   * The depth value is increased by 1 for each recursive call so when you 
   * print, you can see at what level each node is located! (this should help 
   * you debug your code by making it easier to check the shape of your BST).
   *
   * TODO: Implement this function
   */

  if (root != NULL){
    depth++;
    printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n", depth, root->key, root->tx, root->ty, root->w, root->h, root->wsplit);
    BST_preorder(root->left, depth);
    BST_preorder(root->right, depth);
  }

  return;
}

///////////////////////////////////////////////////////////////////////////////

void BST_postorder(Quad *root, int depth) {
  /**
   * This function performs a post-order traversal of the BST and prints out 
   * the information for each Quad using this exactly this print statement:
   *
   *  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",...)
   *
   * Obviously, you must provide the variables to the printf function - we're 
   * just giving you the formatting string.
   *
   * The depth value is increased by 1 for each recursive call so when you 
   * print, you can see at what level each node is located! (this should help 
   * you debug your code by making it easier to check the shape of your BST).
   *
   * TODO: Implement this function
   */

  if (root != NULL){
    depth++;
    BST_postorder(root->left, depth);
    BST_postorder(root->right, depth);
    printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n", depth, root->key, root->tx, root->ty, root->w, root->h, root->wsplit);
  }

  return;
}

///////////////////////////////////////////////////////////////////////////////
// NOTE: For the remaining functions, you may assume the following:          //
//       (1) All the Quads are valid (None of them go outside the image)     //
//       (2) They don't overlap  (A pixel will not be in multiple Quads)     //
///////////////////////////////////////////////////////////////////////////////

int get_colour(Image *im, Quad *q) {
  /**
   * Given an image 'im' and a Quad 'q', get the colour we should be assigning
   * to the pixels that are in it, and return it. For the sake of this 
   * assignment, we will say this is *average* colour of all the pixels in 
   * the quad.
   *
   * The pixel data is stored in a one dimensional array called 'data' in the
   * image struct. Make sure you look at the definition of this to understand
   * how the image is stored. Remember that the pixel data is stored in
   * row-major order, so to get the colour for pixel (x,y) you will look at the
   * index
   *
   *                           x + (y * sx)
   *
   * of the array.
   *
   * TODO: Implement this function. You should not be getting any values 
   *       outside the range of the pixels [0-255] if you have implemented 
   *       this correctly.
   */

  int index = 0, total = 0, occurances = 0;

  for (int i = q->tx; i<(q->tx + q->w); i++){
    for (int j = q->ty; j<(q->ty + q->h); j++){
      index = i + (j*q->sx);
      total += im->data[index];
      occurances++;
    }
  }
  return total / occurances;
}

///////////////////////////////////////////////////////////////////////////////

int similar(Image *im, Quad *q, int threshold) {
  /**
   * Given an image 'im', check if the colours in the area corresponding to the 
   * Quad 'q' are all similar. If not, we will have to split it. For the 
   * purpose of this assigment, we say the colours in a Quad are similar if
   *
   *          maxCol - minCol <= threshold
   *
   * where maxCol and minCol are the maximum and minimum values respectively
   * of the pixel colours in the Quad. The threshold is a parameter. This
   * function should return a 0 if the pixels are not similar enough and the
   * Quad needs to be split, and 1 otherwise.
   *
   * TODO: Implement this function
   */

  int maxCol = 0, minCol = 1000, currentCol = 0, index = 0;

  if (threshold > 255){
    threshold = 255;
  }
  else if (threshold < 0){
    threshold = 0;
  }

  for (int i = q->tx; i<(q->tx + q->w); i++){
    for (int j = q->ty; j<(q->ty + q->h); j++){
      index = i + (j*q->sx);
      currentCol = im->data[index];
      if (currentCol > maxCol){
        maxCol = currentCol;
      }
      if (currentCol < minCol){
        minCol = currentCol;
      }
    }
  }

  if ((maxCol - minCol) <= threshold){
    return 1;
  }
  else{
    return 0;
  }
}

///////////////////////////////////////////////////////////////////////////////

void number_of_quads(Quad *root, int *size){
  //Count and return the total number of quads in the BST
	if (root != NULL){
		number_of_quads(root->left, size);
		*(size) += 1;
		number_of_quads(root->right, size);
	}
	return;
}

void get_quads(Quad *root, int my_array[], int *count){
	//Return an array of pointers that point to each quad in the BST
	if (root != NULL){
		get_quads(root->left, my_array, count);
		my_array[*(count)] = root->key;
		*(count) += 1;
		get_quads(root->right, my_array, count);
	}	
	return;	
}

Quad *key_search(Quad *root, int key){
  //Looks through BST for the quad with the same key as "key" and returns it.
  if (root == NULL){
    return NULL;
  }
  if (root->key == key){
    return root;
  }
  if (root->key < key){
    return key_search(root->right, key);
  }
  else{
    return key_search(root->left, key);
  }
  return NULL;
}


Quad *split_tree(Image *im, Quad *root, int threshold) {
  /**
   * This function traverses the BST, and for each existing Quad, checks if 
   * the pixels in the quad are of a similar colour using the similar() function 
   * and the given threshold. If they are not, then the Quad needs to be split 
   * into 2 Quads(which will hopefully have pixels that are more similar to 
   * each other).
   *
   * To do this, first we need to decide in which direction we are going to 
   * split the Quad. For this, we will use the 'wsplit'field.
   *
   *    - If wsplit = 1, then we split it along the width (ie, we will now have 
   *                     2 quads with the same heightand half the width as the 
   *                     original one)
   *
   *    - If wsplit = 0, then we split along the height.
   *
   * NOTE: We don't always want to split the Quads in the same direction
   *       every time this function is called, because then we could just
   *       end up with very thin and long/tall quads, which wouldn't be very
   *       helpful to what we are trying to do. So, we need to make sure that
   *       once we split a Quad, that we invert the value of the 'wsplit'
   *       variable in both the new nodes, so they split the other way.
   *
   * -------------------------------------------------------------------------
   *
   * For example, if our Quad had the following values:
   *    (tx:ty = 0:0     w = 512,   h = 512,  wsplit = 1) ---> A
   *
   *                (0,0)
   *                   x-------------------------
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |           A            |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   -------------------------x
   *                                         (512, 512)        
   *                
   *                                     * this pixel is not IN the image, just
   *                                       represents the 'corner'. The bottom
   *                                       right pixel, as always, is (511,511)
   * 
   * it would be split along the width, and the resulting two Quads
   * we would get would be as follows:
   *
   *     (tx:ty =  0 :0     w = 256,   h = 512,  wsplit = 0) ---> B
   *     (tx:ty = 256:0     w = 256,   h = 512,  wsplit = 0) ---> C
   *
   *
   *                (0,0)       (256, 0)
   *                   x-----------x-------------
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   |     B     |      C     |
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   -------------------------x
   *                                         (512, 512)
   *
   *   - Note that we want to always split it in exactly half, but if the
   *     width/height is an odd number then round down.
   *
   *   - Further note that 'wsplit' on both of these has now been set to 0.
   *     If they were split again, the resulting Quads would have wsplit = 1.
   *
   * --------------------------------------------------------------------------
   *
   * Now, once you know how it needs to be split, carefully form these two
   * Quads, with the correct positions and sizes, and replace the the original 
   * one with them.
   *
   * This function is crunchy - and if you don't think it through before you 
   * start implementing it you'll run into all kinds of trouble.
   *
   * This is the problem solving exercise for A2, so don't look for people
   * on Piazza to give you answers, or tell you what to do, or verify you're
   * doing the right thing.
   *
   * It's up to you how to solve this, and if you want an opinion, you can
   * come to visit us during office hours! The included file `point.pgm` is
   * a good candidate image to test this function on.
   *
   * Expected result: The BST will have at most twice as many Quads
   *                  as before, depending on how many of them needed to be 
   *                  split.
   *
   * TODO: Implement this function
   */
    
    Quad *new_quad1 = NULL;
    Quad *new_quad2 = NULL;
    Quad *my_quad = NULL;
    int total = 0, count = 0, new_width = 0, new_height = 0, new_x = 0, new_y = 0, current_key = 0;
    int *p_total = NULL;
    int *p_count = NULL;
    p_total =& total;
    p_count =& count;

    if (threshold > 255){
      threshold = 255;
    }
    else if (threshold < 0){
      threshold = 0;
    }

    number_of_quads(root, p_total);
    int p_array[total];
    get_quads(root, p_array, p_count);

    for (int i = 0; i < total; i++){  
      current_key = p_array[i];
      my_quad = key_search(root, current_key);
      //Quad needs to be split
      if (similar(im, my_quad, threshold) == 0){
        //Split along width
        if (my_quad->wsplit == 1){
          new_width = my_quad->w / 2;
          new_x = my_quad->tx + new_width;
          new_quad1 = new_Quad(my_quad->tx, my_quad->ty, new_width, my_quad->h, 0, my_quad->sx);
          new_quad2 = new_Quad(new_x, my_quad->ty, new_width, my_quad->h, 0, my_quad->sx);
          root = BST_delete(root, my_quad->tx, my_quad->ty);
          root = BST_insert(root, new_quad1);
          root = BST_insert(root, new_quad2);
        }
        //Split along height
        else if (my_quad->wsplit == 0){
          new_height = my_quad->h / 2;
          new_y = my_quad->ty + new_height;
          new_quad1 = new_Quad(my_quad->tx, my_quad->ty, my_quad->w, new_height, 1, my_quad->sx);
          new_quad2 = new_Quad(my_quad->tx, new_y, my_quad->w, new_height, 1, my_quad->sx);
          root = BST_delete(root, my_quad->tx, my_quad->ty);
          root = BST_insert(root, new_quad1);
          root = BST_insert(root, new_quad2);
        }
      }
    }

    return root;
}

///////////////////////////////////////////////////////////////////////////////

void drawOutline(Image *im, Quad *root, unsigned char col) {
  /**
   * Given an image 'im' and a BST rooted at 'root', traverse through each quad 
   * and draw an outline for it. The outline consists of the outermost pixels 
   * of the Quad (ie, the top and bottom rows, and the leftmost and rightmost 
   * columns).
   * 
   * Make sure that these outlines are of the input colour 'col' that is passed 
   * in. The colour of the remaining pixels should not be changed. 
   *
   * TODO: Implement this function
   */

  Quad *my_quad = NULL;
  int total = 0, count = 0, index = 0, current_key = 0;
  int *p_total = NULL;
  int *p_count = NULL;
  p_total =& total;
  p_count =& count;

  number_of_quads(root, p_total);
  int p_array[total];
  get_quads(root, p_array, p_count);
  
  for (int i = 0; i < total; i++){
    current_key = p_array[i];
    my_quad = key_search(root, current_key);

    //Top Row
    for (int j = my_quad->tx; j < my_quad->tx+my_quad->w; j++){
      index = j + (my_quad->ty*my_quad->sx);
      im->data[index] = col;
    }
    //Bottom Row
    for (int k = my_quad->tx; k < my_quad->tx+my_quad->w; k++){
      index = k + ((my_quad->ty-1 + my_quad->h)*my_quad->sx);
      im->data[index] = col;
    }
    //Left Column
    for (int l = my_quad->ty; l < my_quad->ty+my_quad->h; l++){
      index = my_quad->tx + (l*my_quad->sx);
      im->data[index] = col;
    }
    //Right Column
    for (int m = my_quad->ty; m < (my_quad->ty+my_quad->h); m++){
      index = ((my_quad->tx-1)+my_quad->w) + (m*my_quad->sx);
      im->data[index] = col;
    }
  }
  return;
}

///////////////////////////////////////////////////////////////////////////////

void save_Quad(Image *im, Quad *root) {
  /**
   * Given an image 'im' and a BST rooted at 'root', traverse through each 
   * quad, and set all the pixels in the corresponding area to the expected 
   * colour of the quad computed by your function get_colour().
   *
   * Make sure you index into the pixels array correctly and change the colour 
   * in the image itself.
   *
   * TODO: Implement this function
   */
  

  Quad *my_quad = NULL;
  int total = 0, count = 0, index = 0, colour = 0, current_key = 0;
  int *p_total = NULL;
  int *p_count = NULL;
  p_total =& total;
  p_count =& count;

  number_of_quads(root, p_total);
  int p_array[total];
  get_quads(root, p_array, p_count);

  for (int i = 0; i < total; i++){
    current_key = p_array[i];
    my_quad = key_search(root, current_key);
    colour = get_colour(im, my_quad);

    for (int x = my_quad->tx; x<(my_quad->tx+my_quad->w); x++){
      for (int y = my_quad->ty; y<(my_quad->ty+my_quad->h); y++){
        index = x + (y*my_quad->sx);
        im->data[index] = colour;      
      }
    }
  }
  return;
}

///////////////////////////////////////////////////////////////////////////////
// That's it, you're done!
