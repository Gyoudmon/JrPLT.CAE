#lang scribble/base

@require{literacy.rkt}

@(define (t:keyword kw)
   (define parts (regexp-match #px"(\\w+)[(](\\w+)[)]" kw))
   (if (and (list? parts) (> (length parts) 2))
       (list (racketresultfont (cadr parts))
             (smaller
              (racketparenfont "(")
              (racketkeywordfont (caddr parts))
              (racketparenfont ")")))
       (racketresultfont kw)))

@(define t:english racketcommentfont)
@(define t:prolang racketoutput)

@(define table-format
   (lambda rows
     (for/list ([row (in-list rows)])
       (list (t:keyword (car row))
             (t:english (cadr row))
             (t:prolang (caddr row))))))

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@handbook-title{词汇表}

@handbook-scenario{语句关键字}

@tabular[#:sep (hspace 1)
 (cons (list @bold{关键字(全称)} @bold{英文本意} @bold{程序语义})
       (table-format (list "auto(automatic)" "自动的" "自行确定类型")
                     (list "break" "折断、停顿" "退出循环")
                     (list "case" "情况、案例" "多重选择的条件分支")
                     (list "cast" "角色扮演、演员" "类型转换")
                     (list "catch" "捕捉、赶上" "捕获待处理异常")
                     (list "class" "类型，引申为班级、阶级" "自定义类型")
                     (list "const(constant)" "恒定不变的" "常量")
                     (list "continue" "继续" "终止当前循环、开始下一轮循环")
                     (list "default" "亏欠" "多重选择的预设分支")
                     (list "delete" "删除" "销毁一个类的实例")
                     (list "do" "干活" "直到型循环的开始")
                     (list "else" "其他、额外" "条件分支的其他情形")
                     (list "for(for each)" "对于……中的每一个" "迭代型循环的开始")
                     (list "friend" "朋友" "声明类/函数的友元类/函数")
                     (list "if" "如果" "条件分支的开始")
                     (list "namespace" "-" "名字空间，该空间内不能同名")
                     (list "new" "新的" "创建一个类的实例")
                     (list "noexcept" "-" "声明函数不会抛出异常")
                     (list "operator" "操作员" "操作符函数重载")
                     (list "override" "否决" "在子类中覆盖父类方法")
                     (list "private" "私有的" "类的私有方法")
                     (list "protected" "被保护的" "类及其子类的私有方法")
                     (list "public" "公共的" "类的公有方法")
                     (list "return" "返回" "带着返回值正常退出函数")
                     (list "static" "静态的" "类的方法、模块的私有函数")
                     (list "struct(structure)" "结构" "class 的同义词(有细微差别)")
                     (list "switch" "开关、交换" "多重选择分枝结构的开始")
                     (list "template" "样板、模版" "定义模版类和模版函数")
                     (list "this" "这个" "类的函数内部的固有变量，始终指向该类的实例")
                     (list "throw" "抛、扔" "强行终止函数，并抛出一个异常/错误")
                     (list "try" "尝试" "尝试执行一段可能抛出异常的代码，没异常最好，有就捕获")
                     (list "typedef" "-" "给类型起一个别名")
                     (list "typeid" "-" "在运行时获取数值的具体类型")
                     (list "typename" "-" "在定义模版时声明范类型的名字")
                     (list "union" "联合、联邦" "声明一个联合类型，联合中的多个类型只能同时存在一个")
                     (list "using" "使用中" "声明当前正处在哪个名字空间中(可多次声明)")
                     (list "virtual" "虚拟的" "声明类的方法为虚函数")
                     (list "while" "当……" "指示“直到型循环”和“当型循环”的执行条件")))]

@handbook-scenario{类型关键字}

@handbook-appendix-tabular/2[
 (list @bold{关键字(全称)} @bold{英文本意} @bold{程序语义})
 (table-format (list "bool(boolean)" "布尔代数" "布尔类型")
               (list "char(character)" "性格、字" "字符类型")
               (list "double" "两倍" "双精度浮点类型")
               (list "false" "假的" "布尔类型的值：假")
               (list "float" "浮动" "单精度浮点类型")
               (list "int(integer)" "整数" "整数类型")
               (list "long" "长的" "长整数类型")
               (list "nullptr" "-" "空指针值")
               (list "short" "短的" "短整数类型")
               (list "signed" "-" "整数类型可带正负号")
               (list "string" "细绳、弦、串" "字符串类型")
               (list "true" "真的" "布尔类型的值：真")
               (list "unsigned" "-" "整数类型不带正负号")
               (list "void" "空虚、无效" "无类型或类型不重要"))]

@handbook-scenario{常用于命名的英语单词}
