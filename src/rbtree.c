#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

// rbtree_display: RB 트리 출력 함수
void rbtree_display(rbtree *t, node_t *node) {
  node_t *cur_node = node;
  printf("Node [%d][%d] > ", cur_node->key, cur_node->color);
  if (cur_node->left != t->nil) printf("Left [%d][%d] ", cur_node->left->key, cur_node->left->color);
  if (cur_node->right != t->nil) printf("Right [%d][%d] ", cur_node->right->key, cur_node->right->color);
  printf("\n");
  if (cur_node->left != t->nil) rbtree_display(t, cur_node->left);
  if (cur_node->right != t->nil) rbtree_display(t, cur_node->right);
}

// new_rbtree: RB 트리 구조체 생성 함수
rbtree *new_rbtree(void) {
  // TODO: initialize struct if needed
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t)); // new_node 메모리 할당

  new_node->color = RBTREE_BLACK; // RBTREE_BLACK
  p->nil = new_node;
  p->root = p->nil;

  return p;
}

// delete_rbtree: RB 트리 구조체가 차지했던 메모리 반환 함수
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  node_t *p = t->root;
  if (p != t->nil)
    delete_node(t, p);

  free(t->nil);
  t->nil = NULL;

  free(t);
}

// delete_node: RB 트리의 모든 노드가 차지했던 메모리 반환 함수
void delete_node(rbtree *t, node_t *node) {
  if (node == t->nil)
    return;

  // deleting left subtree
  delete_node(t, node->left);

  // deleting right subtree
  delete_node(t, node->right);

  // deleting node
  free(node);
}

// rbtree_insert: RB 트리에 값 추가 함수
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  // 새로운 노드 메모리 할당하고 key값과 color 설정 해두기
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  new_node->key = key;

  node_t *p, *parent;
  
  p = t->root; // 포인터는 루트에서 시작
  parent = t->nil; // parent는 nil로 시작

  while (p != t->nil) { // 검색과 동일
    parent = p;
    if (new_node->key < p->key) // 삽입할 값이 p의 값보다 작으면
      p = p->left;
    else  // 삽입할 값이 p의 값보다 크면
      p = p->right;
  }
  new_node->parent = parent;

  if (parent == t->nil) // 트리가 비었을 때
    t->root = new_node;
  else if (new_node->key < parent->key) // 삽입할 값이 parent의 값보다 작으면
    parent->left = new_node;
  else  // 삽입할 값이 parent의 값보다 크면
    parent->right = new_node;
  
  new_node->color = RBTREE_RED;
  new_node->left = t->nil;
  new_node->right = t->nil;
  // RB-INSERT-FIXUP 함수 실행
  rbtree_insert_fixup(t, new_node);

  return t->root;
}

// rbtree_insert_fixup: 삽입 후 속성위반 처리 함수
void rbtree_insert_fixup(rbtree *t, node_t *new_node) {
  node_t *uncle;

  while (new_node->parent->color == RBTREE_RED) // parent의 색이 Red이면 계속 loop를 돈다.
  { 
    if (new_node->parent == new_node->parent->parent->left) { // 왼쪽 case
      uncle = new_node->parent->parent->right; // 삼촌을 uncle로 설정
    
      if (uncle->color == RBTREE_RED) { // 삼촌이 Red라면 -> Case1 : 삼촌이 Red, Parent도 Red
        new_node->parent->color = RBTREE_BLACK; // Case1 해결책
        uncle->color = RBTREE_BLACK;  // Parent와 삼촌을 Black으로 바꾸고
        new_node->parent->parent->color = RBTREE_RED; // 할아버지를 Red로 바꾼 뒤
        new_node = new_node->parent->parent;  // 할아버지에서 다시 확인 시작
      }

      else {
        if (new_node == new_node->parent->right) { // new_node가 parent의 오른쪽 chlid -> Case2 : Parent가 할아버지의 왼쪽 child
          new_node = new_node->parent; // Case2 해결책 : Parent를 기준으로 왼쪽으로 회전한다.
          left_rotate(t, new_node);
        } 
      // Case3 : new_node가 parent의 왼쪽 child, 삼촌이 Black
        new_node->parent->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        right_rotate(t, new_node->parent->parent);
      }
    }
    else {  // 오른쪽 case
      uncle = new_node->parent->parent->left; // 삼촌을 uncle로 설정

      if (uncle->color == RBTREE_RED) { // 삼촌이 Red라면 -> Case1 : 삼촌이 Red, Parent도 Red
        new_node->parent->color = RBTREE_BLACK;  // Case1 해결책
        uncle->color = RBTREE_BLACK;  // Parent와 삼촌을 Black으로 바꾸고
        new_node->parent->parent->color = RBTREE_RED; // 할아버지를 Red로 바꾼 뒤
        new_node = new_node->parent->parent;  // 할아버지에서 다시 확인 시작
      }

      else {
        if (new_node == new_node->parent->left) {  // new_node가 parent의 왼쪽 child -> Case2 : Parent가 할아버지의 오른쪽 Child
          new_node = new_node->parent;  // Case2 해결책 : Parent를 기준으로 오른쪽으로 회전한다.
          right_rotate(t, new_node);
        }
      // Case3: new_node가 Parent의 오른쪽 child, 삼촌이 Black
      new_node->parent->color = RBTREE_BLACK;
      new_node->parent->parent->color = RBTREE_RED;
      left_rotate(t, new_node->parent->parent);
      }
    }
  }  
  t->root->color = RBTREE_BLACK;
}

// left_rotate: 트리 좌회전 함수
                                          // 좌회전   x      =>         y
                                          //       / |               / |
void left_rotate(rbtree *t, node_t *x) {  //      a  y              x  c
  node_t *y;                              //        /|             /|
  y = x->right;                           //       b c            a b
  x->right = y->left; // y의 left subtree(b)를 x의 right subtree로 옮긴다
  
  if (y->left != t->nil) {  // y가 left subtree(b)를 가지고 있는 경우
    y->left->parent = x;    // b를 x의 right subtree로 이동
  }
  y->parent = x->parent; // x의 parent를 y의 parent로 연결

  if (x->parent == t->nil) {  // x가 root였으면 y를 root로 바꾸기
    t->root = y;
  }
  else if (x == x->parent->left) {  // x가 x의 parent의 왼쪽 child였던 경우
    x->parent->left = y;
  }
  else {  // x가 x의 parent의 오른쪽 child였던 경우
    x->parent->right = y;
  }

  y->left = x;  // x를 y의 left로 둔다
  x->parent = y;  // x의 parent를 y로 둔다.
}

// right_rotate: 트리 우회전 함수
                                          // 우회전   x      <=         y
                                          //       / |               / |
void right_rotate(rbtree *t, node_t *y) { //      a  y              x  c
  node_t *x;                              //        /|             /|
  x = y->left;                            //       b c            a b
  y->left = x->right; // x의 right subtree(b)를 y의 left subtree로 옮긴다

  if (x->left != t->nil) { // x가 right subtree(b)를 가지고 있는 경우
    x->right->parent = y;   // b를 y의 left subtree로 이동
  }
  x->parent = y->parent;  // y의 parent를 x의 parent로 연결

  if (y->parent == t->nil) {  // y가 root였으면 x를 root로 바꾸기
    t->root = x;
  }
  else if (y == y->parent->right) { // y가 y의 parent의 오른쪽 child였던 경우
    y->parent->right = x;
  }
  else {  // y가 y의 parent의 왼쪽 child였던 경우
    y->parent->left = x;
  }

  x->right = y; // y를 x의 right로 둔다.
  y->parent = x;  // y의 parent를 x로 둔다. 
}

// inorder_tree_walk: 중위순회로 트리 값 출력 -> 오름차순으로 출력
void inorder_tree_walk(rbtree *t, node_t *root) {
  if (root == t->nil) 
    return;
  inorder_tree_walk(t, root->left);
  printf("(%d %d)\n", root->key, root->color);
  inorder_tree_walk(t, root->right);
}

// rbtree_find: 트리에서 key 있는 지 탐색
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *p = t->root;
  while (p != t->nil)
  { 
    if (p->key == key) {
      return p;
    }
    else if (p->key < key) {
      p = p->right;
    }
    else {
      p = p->left;
    }
  }
  return NULL;
}

// rbtree_min: RB tree 중 최소 값을 가진 node pointer 반환
node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *p = t->root;

  while (p->left != t->nil) {
    p = p->left;
  }

  return p;
}

// rbtree_max: RB tree 중 최대 값을 가진 node pointer 반환
node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *p = t->root;

  while (p->right != t->nil) {
    p = p->right;
  }

  return p;
}

// rbtree_erase: RB tree 내부의 ptr로 지정된 node를 삭제하고 메모리 반환
int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  // p : 삭제할 노드
  // y : 실제로 삭제되는 노드
  // x : successor 노드
  // y_original_color : 실제로 삭제되는 노드의 기존 색깔
  node_t *y, *x;

  // [[[ 삭제할 노드가 실제로 삭제되는 경우 ]]]
  y = p;  // 실제로 삭제되는 노드를 삭제할 노드로 설정
  color_t y_original_color = y->color;  // 삭제되는 노드의 색 저장하기  
  //  #### p의 왼쪽 자식이 없는 경우 ####
  if (p->left == t->nil) { 
    x = p->right; // p의 right를 x 즉 successor로 설정
    rbtree_transplant(t, p, p->right);  // 삭제하려는 p노드 자리에 p의 right 값을 넣어줌 = p를 p->right로 교체
    // if ((p == t->root) && (p->right == t->nil)) { // 둘다 자식이 없는 경우 t->root를 t->nil로 바꿔줘야 함
    //   t->root = t->nil;
    // }
  }
  //  #### p의 오른쪽 자식이 없는 경우 ####
  else if (p->right == t->nil) {
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }

  // [[[ 실제로 삭제되는 노드가 삭제할 노드의 successor인 경우 ]]]
  else {
    y = rbtree_minimum(t, p->right);  // 실제로 삭제되는 노드인 y를 p의 right subtree에서 가장 작은 값으로 설정
    y_original_color = y->color;
    x = y->right; // x는 y자리로 들어갈 successor이다, y는 가장 작은 값이므로 y의 left는 nil이다.
    // ### p의 right 노드가 left를 가지고 있지 않아서 y가 p의 child일 경우 ###
    if (y->parent == p) {
      x->parent = y;
    }
    // ### 일반적인 경우 ###
    else {
      rbtree_transplant(t, y, y->right);  // y자리를 y->right가 대체한다. y값은 z자리로 갔기 때문에 y자리에 y->right를 넣어준다, nil이든 말든 상관 없음
      y->right = p->right;  // y를 p자리로 옮기기
      y->right->parent = y;
    }

    rbtree_transplant(t, p, y); // p자리에 y값 넣기
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if (y_original_color == RBTREE_BLACK) {// 삭제되는 노드의 색이 Black이면 rbtree_delete_fixup 실행
    rbtree_delete_fixup(t, x);
  }
  free(p);
  return 0;
}

// rbtree_delete_fixup: Successor가 Doubly Black인 경우를 해결하는 함수
void rbtree_delete_fixup(rbtree *t, node_t *x) {
  // x: successor
  // w: successor(x)의 형제
  node_t *w;

  while ((x != t->root) && (x->color == RBTREE_BLACK)) { // successor가 루트가 아니고 Black인 경우 while문 돈다. 

    // [[[ 왼쪽 Case ]]]
    if (x == x->parent->left) {
      w = x->parent->right; // w는 x의 오른쪽 형제 -> 왼쪽 Case 이므로

      // ### DB Case1 ###
      if (w->color == RBTREE_RED) { // Case1 : Doubly Black의 형제가 Red인 경우
        w->color = RBTREE_BLACK;  // 형제의 색을 Black으로 바꿈
        x->parent->color = RBTREE_RED;  // 부모의 색을 Red로 바꿈
        left_rotate(t, x->parent);  // 부모를 기준으로 왼쪽으로 회전
        w = x->parent->right; // x의 형제를 재설정 -> 해결 안됐을 경우 다시 while loop 돌아서 Case 2, 3, 4로 해결
      }

      // ### DB Case2 ###
      if ((w->left->color == RBTREE_BLACK) && (w->right->color == RBTREE_BLACK)) { // Case2 : 형제가 Black이고, 그 형제의 두 자녀가 모두 Black인 경우
        w->color = RBTREE_RED;  // 형제의 색을 Red로 바꿈
        x = x->parent;  // successor가 가지고 있던 Doubly Black을 부모에게 전달 -> 해결 안됐을 경우 다시 while loop 돌아서 Case 1, 2, 3, 4로 해결
      }

      // ### DB Case3,4 ###
      else {
        // ### DB Case3 ###
        if (w->right->color == RBTREE_BLACK) {  // Case3 : 형제가 Black이고, 형제의 left가 Red, 형제의 right가 Black인 경우
          w->left->color = RBTREE_BLACK;  // 형제의 left와 형제의 색을 서로 바꾼다.
          w->color = RBTREE_RED;          // 형제의 left는 Black으로, 형제는 Red로
          right_rotate(t, w); // 형제를 기준으로 오른쪽으로 회전
          w = x->parent->right; // x의 형제를 재설정
        }
        // ### DB Case4 ### -> Case4가 진행되면 RB트리가 완성되므로 while문 다시 돌 필요 없음
        // Case3 : 형제가 Black이고, 형제의 right가 Red인 경우
        w->color = x->parent->color;  // 형제의 색을 부모의 색으로 바꾸기
        x->parent->color = RBTREE_BLACK;  // 부모의 색과 형제의 right의 색을 모두 Black으로 바꾼다
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent); // 부모를 기준으로 왼쪽으로 회전
        x = t->root;  // while문이 종료된 후에 x->color = RBTREE_BLACK 이라는 코드가 실행됨
                      // 해당 코드가 실행되면 RB트리가 깨질 수 있음 (x가 Red일 때 Black으로 바꿔서) -> 따라서 x를 루트로 바꿔버려서 그런 일이 일어나지 않도록 함  
      }
    }

    // [[[ 오른쪽 Case ]]]
    else {
      w = x->parent->left; // w는 x의 왼쪽 형제 -> 오른쪽 Case 이므로

      // ### DB Case1 ###
      if (w->color == RBTREE_RED) { // Case1 : Doubly Black의 형제가 Red인 경우
        w->color = RBTREE_BLACK;  // 형제의 색을 Black으로 바꿈
        x->parent->color = RBTREE_RED;  // 부모의 색을 Red로 바꿈
        right_rotate(t, x->parent);  // 부모를 기준으로 오른쪽으로 회전
        w = x->parent->left; // x의 형제를 재설정 -> 해결 안됐을 경우 다시 while loop 돌아서 Case 2, 3, 4로 해결
      }

      // ### DB Case2 ###
      if ((w->right->color == RBTREE_BLACK) && (w->left->color == RBTREE_BLACK)) { // Case2 : 형제가 Black이고, 그 형제의 두 자녀가 모두 Black인 경우
        w->color = RBTREE_RED;  // 형제의 색을 Red로 바꿈
        x = x->parent;  // successor가 가지고 있던 Doubly Black을 부모에게 전달 -> 해결 안됐을 경우 다시 while loop 돌아서 Case 1, 2, 3, 4로 해결
      }

      // ### DB Case3,4 ###
      else {
        // ### DB Case3 ###
        if (w->left->color == RBTREE_BLACK) {  // Case3 : 형제가 Black이고, 형제의 right가 Red, 형제의 left가 Black인 경우
          w->right->color = RBTREE_BLACK;  // 형제의 right와 형제의 색을 서로 바꾼다.
          w->color = RBTREE_RED;          // 형제의 right는 Black으로, 형제는 Red로
          left_rotate(t, w); // 형제를 기준으로 왼쪽으로 회전
          w = x->parent->left; // x의 형제를 재설정
        }
        // ### DB Case4 ### -> Case4가 진행되면 RB트리가 완성되므로 while문 다시 돌 필요 없음
        // Case3 : 형제가 Black이고, 형제의 right가 Red인 경우
        w->color = x->parent->color;  // 형제의 색을 부모의 색으로 바꾸기
        x->parent->color = RBTREE_BLACK;  // 부모의 색과 형제의 left의 색을 모두 Black으로 바꾼다
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent); // 부모를 기준으로 오른쪽으로 회전
        x = t->root;  // while문이 종료된 후에 x->color = RBTREE_BLACK 이라는 코드가 실행됨
      }                // 해당 코드가 실행되면 RB트리가 깨질 수 있음 (x가 Red일 때 Black으로 바꿔서) -> 따라서 x를 루트로 바꿔버려서 그런 일이 일어나지 않도록 함  
    }
  }
  x->color = RBTREE_BLACK;  // x의 색을 Black으로 
  
}


// rbtree_transplant: 삭제하려는 노드의 값을 successor의 값으로 바꾸는 함수
void rbtree_transplant(rbtree *t, node_t *u, node_t *v) { // 삭제하려는 노드 u의 자리에 successor 노드 v의 값을 집어넣는 과정
  if (u->parent == t->nil)  {// 삭제하려는 노드가 루트인 경우
    t->root = v;
  }
  else if (u == u->parent->left) {  // 삭제하려는 노드가 parent의 left인 경우
    u->parent->left = v;
  }
  else {  // 삭제하려는 노드가 parent의 right인 경우  
    u->parent->right = v;
  }
  v->parent = u->parent;  // successor의 parent 설정
}

// rbtree_minimum: p노드가 루트인 subtree에서 minimum 값을 찾는 함수
node_t *rbtree_minimum(rbtree *t, node_t *p) {
  while (p->left != t->nil) {
    p = p->left;
  }
  return p;
}

// rbtree_to_array: RB tree의 내용을 key 순서대로 주어진 array로 변환
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  inorder_array(t, t->root, arr, n, 0);

  return 0;
}

// inorder_array: inorder로 array에 key 순서대로 넣는 함수
int inorder_array(const rbtree *t, node_t *root, key_t *arr, const size_t n, size_t i) {
  if (root == t->nil || i >= n)
    return i;

  i = inorder_array(t, root->left, arr, n, i);
  ////
  arr[i] = root->key;
  i++;
  ////
  i = inorder_array(t, root->right, arr, n, i);

  return i;
}
