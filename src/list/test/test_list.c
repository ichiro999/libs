#include <stdio.h>
#include "list.h"
#include <malloc.h>
#include <string.h>
#include <CUnit/Basic.h>

#define NUM (1024)

typedef struct iterm_t {
    int num;
    char buffer[32];
    struct list_head list;
} iterm_t;


//static LIST_HEAD(g_list);

/*{{{ static void list_add_test(void): test list_add list_for_each_entry and list_for_each_entry */
static void
list_add_test(void) {
    LIST_HEAD(l_list);
    iterm_t *iterm = NULL, *iterm2 = NULL;
    int num = 1023;
    int re_num = 0;
    iterm_t *iter_arr = NULL;

    iter_arr = malloc(NUM * sizeof(iterm_t));
    CU_ASSERT(iter_arr != NULL);
    
    for (int i = 0; i < NUM; i++) {
        
        iter_arr[i].num = i;
        snprintf(iter_arr[i].buffer, sizeof iter_arr[i].buffer, "iterm%d", iter_arr[i].num);
//        printf("<>%d %s----%p %p\n", iter_arr[i].num, iter_arr[i].buffer, &(iter_arr[i].list), &l_list);
        list_add(&(iter_arr[i].list), &l_list);
    }
   //like stack, FILO 
    list_for_each_entry(iterm, &l_list, list) {
       CU_ASSERT(iterm->num == num);
       if (iterm->num != num) {
            printf("<>iterm:%d  =========  %d\n", iterm->num, num);
       }
       num--;
    }

    list_for_each_entry_reverse(iterm2, &l_list, list) {
        CU_ASSERT(iterm2->num == re_num);
        if (iterm2->num != re_num) {
            printf("<>reverse iterm:%d ====== %d\n", iterm2->num, re_num);
        }
        re_num++;
    }
    CU_ASSERT(num == -1);
    CU_ASSERT(re_num == NUM);
    free(iter_arr);
}
/*}}}*/

/*{{{ static void list_add_tail_test(void): test list_add_tail list_for_each_entry and list_for_each_entry */
static void
list_add_tail_test(void) {
    LIST_HEAD(l_list);
    iterm_t *iterm = NULL; 
    int num = 0;
    iterm_t *iter_arr = NULL;

    iter_arr = malloc(NUM * sizeof(iterm_t));
    CU_ASSERT(iter_arr != NULL);
    
    for (int i = 0; i < NUM; i++) {
        
        iter_arr[i].num = i;
        snprintf(iter_arr[i].buffer, sizeof iter_arr[i].buffer, "iterm%d", iter_arr[i].num);
//        printf("<>%d %s----%p %p\n", iter_arr[i].num, iter_arr[i].buffer, &(iter_arr[i].list), &l_list);
        list_add_tail(&(iter_arr[i].list), &l_list);
    }
   //like stack, FILO 
    list_for_each_entry(iterm, &l_list, list) {
       CU_ASSERT(iterm->num == num);
       if (iterm->num != num) {
            printf("<>iterm:%d  =========  %d\n", iterm->num, num);
       }
       num++;
    }
    CU_ASSERT(num == NUM);
    free(iter_arr);
}
/*}}}*/

/*{{{ static void list_each_del(void): test ist_for_each_entry_safe and list_del */
static void
list_each_del(void) {
    iterm_t l_list_head = {.num = -1, .buffer = "iterm-1", .list = LIST_HEAD_INIT(l_list_head.list),};
    LIST_HEAD(l_list);
    iterm_t *iterm = NULL, *next = NULL;
    struct list_head *pos = NULL;
    int num = 1023;
    int num_lest = 0;
    int res = 0;
    iterm_t *iter_arr = NULL;

    iter_arr = malloc(NUM * sizeof(iterm_t));
    CU_ASSERT(iter_arr != NULL);
    
    for (int i = 0; i < NUM; i++) {
        
        iter_arr[i].num = i;
        snprintf(iter_arr[i].buffer, sizeof iter_arr[i].buffer, "iterm%d", iter_arr[i].num);
//        printf("<>%d %s----%p %p\n", iter_arr[i].num, iter_arr[i].buffer, &(iter_arr[i].list), &l_list);
        list_add(&(iter_arr[i].list), &(l_list_head.list));
    }
   //like stack, FILO 
    list_for_each_entry_safe(iterm, next, &(l_list_head.list), list) {
       CU_ASSERT(iterm->num == num);
       CU_ASSERT(next->num == (num - 1));
       if (iterm->num != num) {
            printf("<>iterm:%d  =========  %d\n", iterm->num, num);
       }
       list_del(&(iterm->list));
       num--;
    }
    
    list_for_each(pos, &(l_list_head.list)) {
        num_lest++;
    }
    CU_ASSERT(num_lest == 0);
    res = list_empty(&(l_list_head.list));
    CU_ASSERT(res == 1);
    CU_ASSERT(num == -1);
    free(iter_arr);
}
/*}}}*/

/*{{{ static void list_replace_test(void): test ist_replace */
static void
list_replace_test(void) {
    iterm_t l_list_head = {.num = -1, .buffer = "iterm-1", .list = LIST_HEAD_INIT(l_list_head.list),};
    iterm_t list_iterm_1 = {.num = 1, .buffer = "iterm1", .list = LIST_HEAD_INIT(l_list_head.list),};
    iterm_t list_iterm_2 = {.num = 2, .buffer = "iterm2", .list = LIST_HEAD_INIT(l_list_head.list),};
    iterm_t list_iterm_3 = {.num = 3, .buffer = "iterm3", .list = LIST_HEAD_INIT(l_list_head.list),};
    iterm_t *iterm = NULL;
    int num = 2;

    list_add(&(list_iterm_1.list), &(l_list_head.list));
    list_add(&(list_iterm_2.list), &(l_list_head.list));
    
    list_for_each_entry(iterm, &(l_list_head.list), list) {
       CU_ASSERT(iterm->num == num);
       if (iterm->num != num) {
            printf("<>iterm:%d  =========  %d\n", iterm->num, num);
       }
       num--;
    }
    
    CU_ASSERT(iterm->num == l_list_head.num);
    list_replace(&(list_iterm_2.list), &(list_iterm_3.list));

    list_for_each_entry(iterm, &(l_list_head.list), list) {
        CU_ASSERT(iterm->num != list_iterm_2.num);
    }
        
}
/*}}}*/

/*{{{ static void list_move_test(void): test ist_move */
static void
list_move_test(void) {
    iterm_t l_list_head = {.num = -1, .buffer = "iterm-1", .list = LIST_HEAD_INIT(l_list_head.list),};
    iterm_t l_list_head_2 = {.num = -1, .buffer = "iterm-1", .list = LIST_HEAD_INIT(l_list_head_2.list),};
    iterm_t list_iterm_1 = {.num = 1, .buffer = "iterm1", .list = LIST_HEAD_INIT(l_list_head.list),};
    iterm_t list_iterm_2 = {.num = 2, .buffer = "iterm2", .list = LIST_HEAD_INIT(l_list_head.list),};
    iterm_t list_iterm_3 = {.num = 3, .buffer = "iterm3", .list = LIST_HEAD_INIT(l_list_head.list),};
    iterm_t *iterm = NULL, *first = NULL;
    int num = 2;
    int single = 0;

    list_add(&(list_iterm_1.list), &(l_list_head.list));
    list_add(&(list_iterm_2.list), &(l_list_head.list));
    list_add(&(list_iterm_3.list), &(l_list_head_2.list));
    
    list_for_each_entry(iterm, &(l_list_head.list), list) {
       CU_ASSERT(iterm->num == num);
       if (iterm->num != num) {
            printf("<>iterm:%d  =========  %d\n", iterm->num, num);
       }
       num--;
    }
    CU_ASSERT(iterm->num == l_list_head.num);
    
    single = list_is_singular(&(l_list_head_2.list));
    CU_ASSERT(single == 1);

    list_move(&(list_iterm_2.list), &(l_list_head_2.list));

    single = 0;
    single = list_is_singular(&(l_list_head.list));
    CU_ASSERT(single == 1);

    list_for_each_entry(iterm, &(l_list_head.list), list) {
        CU_ASSERT(iterm->num != list_iterm_2.num);
    }

    single = 1;
    single = list_is_singular(&(l_list_head_2.list));
    CU_ASSERT(single == 0);

    num = 0;
    list_for_each_entry(iterm, &(l_list_head_2.list), list) {
        num++;
    }
    first = list_first_entry(&(l_list_head_2.list), typeof(*first), list);
    CU_ASSERT(first->num == list_iterm_2.num);
    CU_ASSERT(num == 2);        
}
/*}}}*/

/*{{{ static void list_cut_position_test(void): test ist_cut_position */
static void
list_cut_position_test(void) {
    iterm_t l_list_head = {.num = -1, .buffer = "iterm-1", .list = LIST_HEAD_INIT(l_list_head.list),};
    iterm_t l_list_head_2 = {.num = -1, .buffer = "iterm-1", .list = LIST_HEAD_INIT(l_list_head_2.list),};
    iterm_t l_list_head_3 = {.num = -1, .buffer = "iterm-1", .list = LIST_HEAD_INIT(l_list_head_3.list),};
    iterm_t list_iterm_1 = {.num = 1, .buffer = "iterm1", .list = LIST_HEAD_INIT(l_list_head.list),};
    iterm_t list_iterm_2 = {.num = 2, .buffer = "iterm2", .list = LIST_HEAD_INIT(l_list_head.list),};
    iterm_t list_iterm_3 = {.num = 3, .buffer = "iterm3", .list = LIST_HEAD_INIT(l_list_head.list),};
    iterm_t list_iterm_4 = {.num = 4, .buffer = "iterm4", .list = LIST_HEAD_INIT(l_list_head.list),};
    iterm_t list_iterm_5 = {.num = 5, .buffer = "iterm5", .list = LIST_HEAD_INIT(l_list_head.list),};
    iterm_t *iterm = NULL;
    int num = 5;
    int res = 0;

    list_add(&(list_iterm_1.list), &(l_list_head.list));
    list_add(&(list_iterm_2.list), &(l_list_head.list));
    list_add(&(list_iterm_3.list), &(l_list_head.list));
    list_add(&(list_iterm_4.list), &(l_list_head.list));
    list_add(&(list_iterm_5.list), &(l_list_head.list));
    
    list_for_each_entry(iterm, &(l_list_head.list), list) {
       CU_ASSERT(iterm->num == num);
       if (iterm->num != num) {
            printf("<>iterm:%d  =========  %d\n", iterm->num, num);
       }
       num--;
    }
    CU_ASSERT(iterm->num == l_list_head.num);
    res = 1;
    res = list_empty(&(l_list_head_2.list));
    CU_ASSERT(res == 1);
    res = 1;
    res = list_empty(&(l_list_head_3.list));
    CU_ASSERT(res == 1);

    list_cut_position(&(l_list_head_2.list), &(l_list_head_3.list), &(list_iterm_5.list));

    res = 0;
    res = list_empty(&(l_list_head_2.list));
    CU_ASSERT(res == 1);

    list_cut_position(&(l_list_head_2.list), &(l_list_head.list), &(list_iterm_5.list));

    res = 0;
    res = list_is_singular(&(l_list_head_2.list));
    CU_ASSERT(res == 1);

    list_for_each_entry(iterm, &(l_list_head_2.list), list) {
        CU_ASSERT(iterm->num == list_iterm_5.num);
    }

    list_cut_position(&(l_list_head_3.list), &(l_list_head_2.list), &(list_iterm_4.list));

    res = 0;
    res = list_empty(&(l_list_head_3.list));
    CU_ASSERT(res == 1);

    list_cut_position(&(l_list_head_3.list), &(l_list_head_2.list), &(list_iterm_5.list));

    res = 0;
    res = list_empty(&(l_list_head_3.list));
    CU_ASSERT(res == 0);
    list_for_each_entry(iterm, &(l_list_head_3.list), list) {
        CU_ASSERT(iterm->num == list_iterm_5.num);
    }

    //lost the list-iterm_5 at l_list_head_3 list;
    list_cut_position(&(l_list_head_3.list), &(l_list_head.list), &(list_iterm_3.list));
    
    num = 0;
    list_for_each_entry(iterm, &(l_list_head_3.list), list) {
        num++;
    }
    CU_ASSERT(num == 2);

    num = 0;
    list_for_each_entry(iterm, &(l_list_head.list), list) {
        num++;
    }
    CU_ASSERT(num == 2);        
}
/*}}}*/
int main() {

    /*{{{ init CU suite normal_test*/
    CU_pSuite pSuite = NULL;
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    pSuite = CU_add_suite("normal_test", NULL, NULL);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    /**}}}*/

    /**{{{ CU_add_test: list_add_test*/
    if (CU_add_test(pSuite, "list_add_test", list_add_test) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    /*}}}*/
    /**{{{ CU_add_test: list_each_del*/
    if (CU_add_test(pSuite, "list_each_del", list_each_del) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    /*}}}*/
    /**{{{ CU_add_test: list_add_tail_test*/
    if (CU_add_test(pSuite, "list_add_tail_test", list_add_tail_test)== NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    /*}}}*/
    /**{{{ CU_add_test: list_replace_test*/
    if (CU_add_test(pSuite, "list_replace_test", list_replace_test)== NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    /*}}}*/
    /**{{{ CU_add_test: list_move_test*/
    if (CU_add_test(pSuite, "list_move_test", list_move_test)== NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    /*}}}*/
    /**{{{ CU_add_test: list_cut_position_test*/
    if (CU_add_test(pSuite, "list_cut_position_test", list_cut_position_test)== NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    /*}}}*/
    printf("--------------------test------------------------\n");
    /*{{{ CU run & cleanup */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    /*}}}*/
    return CU_get_error();

}
