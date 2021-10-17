//
//  stack.h
//  hstack
//
//  Created by 朱仕哲 on 2021/9/11.
//

#ifndef stack_h
#define stack_h

#define Stack_init_size 100
#define Stack_size_increment 10

#define stack_initialize(stack_type) \
stack_type##_stackbase_ptr = (stack_type*)malloc(Stack_init_size*(sizeof(stack_type)));\
if(stack_type##_stackbase_ptr){\
    stack_type##_stacktop_ptr = stack_type##_stackbase_ptr;\
    stack_type##_stacksize = Stack_init_size;\
}else exit(0)
//最后不用加“;“因为调用函数时会加

#define stack_is_empty(stack_type) \
(stack_type##_stacktop_ptr == stack_type##_stackbase_ptr) ? 1 : 0
//判断是否为空

#define gettop(stack_type,e) \
(stack_type##_stacktop_ptr == stack_type##_stackbase_ptr) ? 0 :\
(e = *(stack_type##_stacktop_ptr - 1), 1)
//获取栈顶元素

#define clear(stack_type) \
stack_type##_stacktop_ptr = stack_type##_stackbase_ptr
//clear

#define destroy(stack_type) \
free(stack_type##_stackbase_ptr)
//destroy

#define stacklenth(stack_type) \
stack_type##_stacktop_ptr - stack_type##_stackbase_ptr
//获取栈长

#define pop(stack_type,e) \
(stack_type##_stacktop_ptr == stack_type##_stackbase_ptr) ? 0 :\
(e = *(--stack_type##_stacktop_ptr), 1)
//出栈

#define push(stack_type,e) \
if((stack_type##_stacktop_ptr - stack_type##_stackbase_ptr) >= stack_type##_stacksize){\
    stack_type##_stackbase_ptr =\
    (stack_type*)realloc(stack_type##_stackbase_ptr,stack_type##_stacksize+\
    Stack_size_increment);\
    if(!stack_type##_stackbase_ptr){\
        exit(0);\
    }\
    stack_type##_stacktop_ptr = stack_type##_stackbase_ptr + stack_type##_stacksize;\
    stack_type##_stacksize += Stack_size_increment;\
}\
*(stack_type##_stacktop_ptr++) = e
//入栈

#define traverse(stack_type,func)
//遍历

#endif /* stack_h */
//i am writing c using vim!
//lalallallal
//git is so cool!

