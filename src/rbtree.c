#include "rbtree.h"  // rbtree 데이터 구조 선언 및 함수 프로토타입을 제공하는 헤더 파일
#include <stdlib.h>  // malloc, free 함수가 선언된 헤더 파일
// -> 표시는 구조체 포인터 변수의 멤버에 접근하기 위해 사용 : t->root는 구조체 포인터 변수 t가 가리키는 구조체의 root멤버에 접근하는 것을 의미

//🔥 rb트리 구조생성(동적 메모리 할당 및 초기화)
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *NIL = (node_t *)calloc(1, sizeof(node_t));

  NIL->color = RBTREE_BLACK;
  p->nil = NIL;
  p->root = NIL;
  
  return p;
}

//🔥 rb트리 삭제(1) : 후위 순회로 주소 할당 해제
void delete_node(rbtree *t, node_t *root) {
  if(root == t->nil)
    return;
  
  if (root->left != t->nil)
    delete_node(t, root->left);
  if (root->right != t->nil)
    delete_node(t, root->right);
  free(root);
}

//🔥 rb트리 삭제(2) : rb트리 삭제
void delete_rbtree(rbtree *t) { 
  delete_node(t, t->root);
  free(t->nil);
  free(t);
}

// 🌸 왼쪽 회전함수
void left_rotation(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;
  
  if (y->left != t->nil)
    y->left->parent = x;
    
  y->parent = x->parent;
  
  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else 
    x->parent->right = y;
    
  y->left = x;
  x->parent = y;
}

// 🌸 오른쪽 회전함수
void right_rotation(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;

  if (y->right != t->nil)
    y->right->parent = x;

  y->parent = x->parent;

  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else 
    x->parent->right = y;

  y->right = x;
  x->parent = y;
}

//🔥 삽입 함수(1) : rb트리 균형을 맞추기 위해 회전 연산 등을 수행
void rbtree_insert_fixup(rbtree *t, node_t *z) {

  while (z->parent->color == RBTREE_RED) {
    // z의 부모가 조부모의 왼쪽 서브 트리일 경우
    if (z->parent == z->parent->parent->left) {
      node_t *y = z->parent->parent->right;
      
      // CASE 1 : 노드 z의 삼촌 y가 적색인 경우
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // CASE 2 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
      else {
        if (z == z->parent->right) {
          z = z->parent;
          left_rotation(t, z);
        }
        // CASE 3 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotation(t, z->parent->parent);
      }
    }
    // z의 부모가 조부모의 왼쪽 서브 트리일 경우
    else {
      node_t *y = z->parent->parent->left;

      // CASE 4 : 노드 z의 삼촌 y가 적색인 경우
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // CASE 5 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
      else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotation(t, z);
        }
        // CASE 6 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotation(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

//🔥 삽입 함수(2) : 적절한 위치를 찾아서 새로운 노드를 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;

  node_t *y = t->nil;
  node_t *x = t->root;

  while (x != t->nil) {
    y = x;
    if (z->key < x->key)
      x = x->left;
    else 
      x = x->right; 
  }

  z->parent = y;
  
  if (y == t->nil)
    t->root = z;
  else if (z->key < y->key)
    y->left = z;
  else
    y->right = z;

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  rbtree_insert_fixup(t, z);

  return z;   
}

//🔥 탐색 함수 : rb트리를 순회하면서 주어진 key값을 가지는 노드를 탐색
node_t *rbtree_find(const rbtree *t, const key_t key) {

  node_t *p = t->root;

  while(p != t->nil)
  {
    if(p->key == key)
      return p;
    else if(p->key > key)
      p = p->left;
    else
      p = p-> right;
  }
  return NULL;
}

//🌸 최소값 : rb트리에서 가장 작은 key값을 가지는 노드를 탐색
node_t *rbtree_min(const rbtree *t) {
  // if (t->root == t->nil)
  //   return NULL;

  node_t *p = t->root;

  while (p->left != t->nil) {
    p = p->left;
  }
  return p;
}

//🌸 최대값 : rb트리에서 가장 큰 key값을 가지는 노드를 탐색
node_t *rbtree_max(const rbtree *t) {
  // if (t->root == t->nil)
  //   return NULL;

  node_t *p = t->root;
  
  while (p->right != t->nil) {
    p = p->right;
  }
  return p;
}

//🔥 노드 삭제(1) : 대체 노드와 삭제 노드 위치 변경
void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  
  v->parent = u->parent;
}

//🔥 노드 삭제(2) : 노드 삭제 후 rb트리 균형을 맞추기 위해 회전 연산 등을 수행
void rbtree_delete_fixup(rbtree *t, node_t *x) {
  while (x != t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) {
      node_t *w = x->parent->right;

      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotation(t, x->parent);
        w = x->parent->right;
      }

      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotation(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotation(t, x->parent);
        x = t->root;
      }
    } else {
      node_t *w = x->parent->left;

      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotation(t, x->parent);
        w = x->parent->left;
      }

      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotation(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotation(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

//🔥 노드 삭제(3) : erase 함수 내에서 min 찾기
node_t *tree_minimum(rbtree *t, node_t *p) {
  while (p->left != t->nil) {
    p = p->left;
  }
  return p;
}

//🔥 노드 삭제(4) : 노드 삭제~~~
int rbtree_erase(rbtree *t, node_t *z) {
  node_t *x, *y;
  color_t y_original_color = z->color;

  if (z->left == t->nil) {
    x = z->right;
    rbtree_transplant(t, z, z->right);
  } else if (z->right == t->nil) {
    x = z->left;
    rbtree_transplant(t, z, z->left);
  } else {
    y = tree_minimum(t, z->right);
    y_original_color = y->color;
    x = y->right;

    if (y->parent == z) {
      x->parent = y;
    } else {
      rbtree_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }

    rbtree_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  if (y_original_color == RBTREE_BLACK) {
    rbtree_delete_fixup(t, x);
  }

  free(z);
  return 1;
}
//🔥 배열 변환(1) : rb트리를 중위순회 
void array_sub(const rbtree *t, node_t *x, key_t *arr, const size_t n, size_t *index) {
  if (x == t->nil)
    return;

  array_sub(t, x->left, arr, n, index);

  if (*index < n)
    arr[(*index)++] = x->key;

  array_sub(t, x->right, arr, n, index);
}

//🔥 배열 변환(2) : 노드의 key값을 배열에 저장
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  if (t->root == t->nil)
    return 0;

  size_t index = 0;
  array_sub(t, t->root, arr, n, &index);

  return 0;
}
