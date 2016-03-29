<?php

class ArticleController extends \BaseController
{


    public function __construct()
    {
        $this->beforeFilter('admin', array('only' => array('create', 'store', 'edit', 'destroy')));
    }

    /**
     * Display a listing of the resource.
     * GET /article
     *
     * @return Response
     */
    public function index()
    {

    }

    /**
     * Show the form for creating a new resource.
     * GET /article/create
     *
     * @return Response
     */
    public function create()
    {
        return View::make('admin.article_create');
    }

    /**
     * Store a newly created resource in storage.
     * POST /article
     *
     * @return Response
     */
    public function store()
    {
        $article = new Article();
        $article->title = Input::get('title');
        $article->tags = Input::get('tags');
        $article->description = Input::get('description');
        $article->content = Input::get('content');
        if (count(Article::where('title', $article->title)->get()) > 0) {
            return Redirect::back()->withInput()->withErrors('存在相同标题的文章');
        }
        try {
            $article->save();
            $tags = Input::get('tags');
            $tagarray = explode(',', $tags);
            foreach ($tagarray as $val) {
                if ($val != "") {
                    $tag = Tag::findOrFail($val);
                    $temp = $tag->article_ids;
                    $temparr = explode(',', $temp);
                    if (!$temp || $temp == "") {
                        $tag->article_ids = $temp . $article->id;
                    }
                    if ($temp && $temp != "" && !in_array($article->id, $temparr)) {
                        $tag->article_ids = $temp . "," . $article->id;
                    }
                    $tag->save();

                }
            }


        } catch (\Exception $e) {
            return Redirect::back()->withInput()->withErrors($e->getMessage());
        }

        return Redirect::route('admin.index');
    }

    /**
     * Display the specified resource.
     * GET /article/{id}
     *
     * @param  int $id
     * @return Response
     */
    public function show($id)
    {
        $article = Article::findOrFail($id);
        if (!Sentry::check()) {
            $article->click_num += 1;
            $article->save();
        }
        $tags = Tag::all();
        $click_articles = Article::orderBy('click_num', 'desc')->take(10)->get();
        $blogtagarray = getOneBlogTagArray($article);
        $comments = $article->comments()->orderBy('created_at', 'desc')->getResults();
        return View::make('home.article')->with('article', $article)->with('tags', $tags)->with('blogtagarray', $blogtagarray)->with('comments', $comments)->with('click_articles', $click_articles);

    }

    /**
     * Show the form for editing the specified resource.
     * GET /article/{id}/edit
     *
     * @param  int $id
     * @return Response
     */
    public function edit($id)
    {
        $article = Article::findOrFail($id);
        return View::make('admin.article_edit')->with('article', $article);

    }

    /**
     * Update the specified resource in storage.
     * PUT /article/{id}
     *
     * @param  int $id
     * @return Response
     */
    //由于未使用eloquent自身的多对多映射，所以代码略微复杂
    public function update($id)
    {
        $article = Article::findOrFail($id);
        $name = Input::get('title');
        if (count(Article::where('title', $name)->where('id', '!=', $id)->get()) > 0) {
            return Redirect::back()->withInput()->withErrors('存在相同标题的文章');
        }
        $origintags = $article->tags;
        $origin_tag_array = explode(',', $origintags);
        $newtags = Input::get('tags');
        $new_tag_array = explode(',', $newtags);


        try {

            //验证新增tag id的存在性

            foreach ($new_tag_array as $val) {
                if ($val != "") {
                    $tagtemp = Tag::findOrFail($val);
                }

            }

            //删除tag表中不应再存在的此article id

            foreach ($origin_tag_array as $val) {
                if ($val != "") {

                    if (!in_array($val, $new_tag_array)) {
                        $tag = Tag::findOrFail($val);
                        $tag_articles = $tag->article_ids;
                        $tag_articles_array = explode(',', $tag_articles);
                        foreach ($tag_articles_array as $key => $val) {
                            if ($val == $article->id) {
                                unset($tag_articles_array[$key]);
                                break;
                            }
                        }
                        $temp = "";
                        foreach ($tag_articles_array as $val) {
                            $temp = $temp . $val . ",";
                        }
                        $temp = rtrim($temp, ',');
                        $tag->article_ids = $temp;
                        $tag->save();
                    }

                }
            }


            //添加tag表中新增加的此article的id

            foreach ($new_tag_array as $val) {
                if ($val != "") {

                    if (!in_array($val, $origin_tag_array)) {
                        $tag = Tag::findOrFail($val);
                        if ($tag->article_ids == "") {
                            $tag->article_ids = $tag->article_ids . $article->id;
                        } else {
                            $tag->article_ids = $tag->article_ids . "," . $article->id;
                        }

                        $tag->save();
                    }

                }
            }

            //更新article
            $article->tags = Input::get('tags');
            $article->title = Input::get('title');
            $article->content = Input::get('content');
            $article->description = Input::get('description');
            $article->save();
            return Redirect::route('article.show', array($article->id));
        } catch (\Exception $e) {
            return Redirect::back()->withInput()->withErrors($e->getMessage());
        }
    }

    /**
     * Remove the specified resource from storage.
     * DELETE /article/{id}
     *
     * @param  int $id
     * @return Response
     */
    public function destroy($id)
    {
        try {
            Article::find($id)->delete();
        } catch (\Exception $e) {

        }
        return Redirect::to(URL::previous());
    }

}
