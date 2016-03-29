<?php

return [

    // 菜单里面显示的名字
    'title' => '标签管理',

    // 右上角有 `New $single` 的创建新内容的文字
    'single' => '标签',

    // 依赖于 Eloquent ORM 作数据读取和处理
    'model' => 'Tag',

    // 显示页面
    'columns' => [

        // 当前列在数据库中的字段名称, 下同
        'id' => [
            // 这个参数定义当前列的名称, 下同
            'title' => 'ID'
        ],
        'name' => [
            'title' => '名称',
        ],

        'article_ids' => [
            'title' => "文章ID",
        ],
        // 不指定 title 的话, 会使用字段作为 title

        'created_at',
    ],

    // 单点击选择单条数据的时候, 右边会出现编辑小视图, 这里定义了视图里面的字段
    'edit_fields' => [

        // 对应字段
        'name' => [

            // 标题
            'title' => '名称',
        ],
        'article_ids' => array(

            'title' => '文章ID',
        ),
    ],

    // 过滤, 搜索
    'filters' => [
        'title' => [
            'title' => '名称',
        ],
        'created_at',
        'created_at',
    ],

];