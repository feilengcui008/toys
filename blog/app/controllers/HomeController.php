<?php

class HomeController extends \BaseController
{

    /**
     * Display a listing of the resource.
     * GET /home
     *
     * @return Response
     */
    public function index()
    {
        $articles = Article::orderBy('created_at', 'desc')->paginate(4);
        $click_articles = Article::orderBy('click_num', 'desc')->take(10)->get();
        $blogtagarray = getBlogTagArray($articles);
        $tags = Tag::all();
        return View::make('home.index')->with('articles', $articles)->with('tags', $tags)->with('blogtagarray', $blogtagarray)->with('click_articles', $click_articles);
    }

    public function about()
    {
        return View::make('home.about');
    }

    public function test()
    {
        /*one to many
        $article = Article::find(1);
        $comments = $article->comments()->getResults();
        return ($comments->toJson());
        $comment = Comment::find(1);
        $post = $comment->article()->getResults();
        return ($comment->toJson());*/

        /* many to many
         $users = Article::find(1)->users()->getResults()->toJson();
         return ($users);
         $articles = User::find(2)->articles()->getResults()->toArray();
         dump($articles);*/


    }


}