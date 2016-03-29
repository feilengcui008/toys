<?php
//美化输出
function dump($var, $echo = true, $label = null, $strict = true)
{
    $label = ($label === null) ? '' : rtrim($label) . ' ';
    if (!$strict) {
        if (ini_get('html_errors')) {
            $output = print_r($var, true);
            $output = '<pre>' . $label . htmlspecialchars($output, ENT_QUOTES) . '</pre>';
        } else {
            $output = $label . print_r($var, true);
        }
    } else {
        ob_start();
        var_dump($var);
        $output = ob_get_clean();
        if (!extension_loaded('xdebug')) {
            $output = preg_replace('/\]\=\>\n(\s+)/m', '] => ', $output);
            $output = '<pre>' . $label . htmlspecialchars($output, ENT_QUOTES) . '</pre>';
        }
    }
    if ($echo) {
        echo($output);
        return null;
    } else
        return $output;
}


//拆开分页对象，得到其所有文章的tag对像数组
function getBlogTagArray($page)
{
    $article_array = $page->toArray()["data"];
    $blogtagarray = array();
    foreach ($article_array as $key => $value) {
        if ($value["tags"]) {
            $idarray = explode(",", $value["tags"]);
            $blogtagarray[$value["id"]] = Tag::whereIn("id", $idarray)->get();
        }

    }
    return $blogtagarray;
}

//返回一篇文章的tag对象数组
function getOneBlogTagArray($article)
{
    $idarray = explode(",", $article->tags);
    $tags = Tag::whereIn('id', $idarray)->get();
    return $tags;
}


