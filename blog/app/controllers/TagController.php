<?php

class TagController extends \BaseController
{

    public function __construct()
    {
        $this->beforeFilter('admin', array('only' => array('create', 'store', 'edit', 'destroy')));
    }

    /**
     * Display a listing of the resource.
     * GET /tag
     *
     * @return Response
     */
    public function index()
    {
    }

    /**
     * Show the form for creating a new resource.
     * GET /tag/create
     *
     * @return Response
     */
    public function create()
    {
        return View::make('admin.tag_create');
    }

    /**
     * Store a newly created resource in storage.
     * POST /tag
     *
     * @return Response
     */
    public function store()
    {
        $tagname = Purifier::clean(Input::get('name'));
        if (count(Tag::where('name', $tagname)->get()) > 0) {
            return Redirect::to(URL::previous())->withErrors('已有相同名称的标签');
        }
        $tag = new Tag();
        $tag->name = $tagname;
        $tag->save();
        return Redirect::route('admin.tag');
    }

    /**
     * Display the specified resource.
     * GET /tag/{id}
     *
     * @param  int $id
     * @return Response
     */
    public function show($id)
    {
        $tag = Tag::findOrFail($id);
        $tags = Tag::all();
        $click_articles = Article::orderBy('click_num', 'desc')->take(10)->get();
        $article_ids = $tag->article_ids;
        $article_ids = explode(',', $article_ids);
        $articles = Article::whereIn('id', $article_ids)->orderBy('updated_at', 'desc')->paginate(4);
        $blogtagarray = getBlogTagArray($articles);
        return View::make('home.index')->with('tags', $tags)->with('articles', $articles)->with('blogtagarray', $blogtagarray)->with('click_articles', $click_articles);

    }

    /**
     * Show the form for editing the specified resource.
     * GET /tag/{id}/edit
     *
     * @param  int $id
     * @return Response
     */
    public function edit($id)
    {
        $tag = Tag::findOrFail($id);
        return View::make('admin.tag_edit')->with('tag', $tag);
    }

    /**
     * Update the specified resource in storage.
     * PUT /tag/{id}
     *
     * @param  int $id
     * @return Response
     */
    public function update($id)
    {
        $tag = Tag::findOrFail($id);
        $tagname = Purifier::clean(Input::get('name'));
        if (count(Tag::where('name', $tagname)->where('id', '!=', $id)->get()) > 0) {
            return Redirect::to(URL::previous())->withErrors('存在相同的标签名');
        }
        $article_ids = Purifier::clean(Input::get('article_ids'));
        $article_ids_array = explode(',', $article_ids);
        try {


            foreach ($article_ids_array as $val) {
                if ($val != '') {
                    $article = Article::find($val);
                    $tags = $article->tags;
                    $tagarr = explode(',', $tags);
                    if ($tags == '' || !$tags) {
                        $tags = $tags . $tag->id;
                    }
                    if ($tags != '' && !in_array($tag->id, $tagarr)) {
                        $tags = $tags . ',' . $tag->id;
                    }
                    $article->tags = $tags;
                    $article->save();
                }
            }
            $tag->name = $tagname;
            $tag->article_ids = $article_ids;
            $tag->save();


        } catch (\Exception $e) {
            return Redirect::back()->withInput()->withErrors($e->getMessage());

        }
        return Redirect::route('admin.tag');
    }

    /**
     * Remove the specified resource from storage.
     * DELETE /tag/{id}
     *
     * @param  int $id
     * @return Response
     */
    public function destroy($id)
    {
        try {
            Tag::find($id)->delete();
        } catch (\Exception $e) {

        }
        return Redirect::to(URL::previous());
    }

}