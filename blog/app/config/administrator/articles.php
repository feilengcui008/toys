<?php

return [

    // 菜单里面显示的名字
    'title' => '文章管理',

    // 右上角有 `New $single` 的创建新内容的文字
    'single' => '文章',

    // 依赖于 Eloquent ORM 作数据读取和处理
    'model' => 'Article',

    // 显示页面
    'columns' => [

        // 当前列在数据库中的字段名称, 下同
        'id' => [
            // 这个参数定义当前列的名称, 下同
            'title' => '文章ID'
        ],
        'title' => [
            'title' => '标题',
        ],
        'content' => [
            'title' => '内容',

            // 这个参数定义了是否支持排序, 下同
            'sortable' => false,

            // 这是自定义输出内容, 如果不定义的话, 会使用字段对应的数据, 下同
            'output' => function ($value) {

                if (mb_strlen($value, 'utf8') > 200) {
                    return mb_substr($value, 0, 200) . '...';
                }
                return $value;
            },
        ],


        'tags' => [
            'title' => "标签ID",
        ],
        // 不指定 title 的话, 会使用字段作为 title
        'updated_at',
        'created_at',

    ],

    // 单点击选择单条数据的时候, 右边会出现编辑小视图, 这里定义了视图里面的字段
    'edit_fields' => [

        // 对应字段
        'title' => [

            // 标题
            'title' => '标题',

            // 可编辑字段的类型
            'type' => 'text'
        ],
        'tags' => array(

            'title' => '标签',
        ),
        'content' => array(
            'title' => '内容',
            'type' => 'textarea',
        ),
//        'comments' => [
//            'title' => 'Comments',
//            'type' => 'relationship'
//        ],
    ],

    // 过滤, 搜索
    'filters' => [
        'title' => [
            'title' => '标题',
        ],
        'updated_at',
        'created_at',
    ],

    // 点击选择某条数据时候, 右上角的链接
    'link' => function ($model) {
        return URL::route('article.show', $model->id);
    },
];