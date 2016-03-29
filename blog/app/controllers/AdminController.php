<?php

class AdminController extends \BaseController
{

    /**
     * Display a listing of the resource.
     * GET /admin
     *
     * @return Response
     */
    public function index()
    {
        $articles = Article::orderBy('updated_at', 'desc')->paginate(20);
        return View::make('admin.index')->with('articles', $articles);
    }

    public function tagAdmin()
    {
        $tags = Tag::orderBy('updated_at', 'desc')->paginate(20);
        return View::make('admin.tag')->with('tags', $tags);
    }

    public function commentAdmin($id)
    {
        $data = Article::with('comments')->find($id);
        return View::make('admin.comment')->with('data', $data);

    }


}