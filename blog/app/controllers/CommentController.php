<?php

class CommentController extends \BaseController
{

    public function __construct()
    {
        $this->beforeFilter('admin', array('only' => array('create', 'edit', 'destroy')));
    }

    /**
     * Display a listing of the resource.
     * GET /comment
     *
     * @return Response
     */
    public function index()
    {
        //
    }

    /**
     * Show the form for creating a new resource.
     * GET /comment/create
     *
     * @return Response
     */
    public function create()
    {
        //
    }

    /**
     * Store a newly created resource in storage.
     * POST /comment
     *
     * @return Response
     */
    public function store()
    {
        $username = Purifier::clean(Input::get('username'));
        $email = Input::get('email');
        $content = Purifier::clean(Input::get('content'));
        $at = Purifier::clean(Input::get('at'));
        $article_id = Purifier::clean(Input::get('article_id'));
        $rules = ['username' => 'required|max:255', 'content' => 'required|max:500', 'email' => 'required|email', 'at' => 'max:100', 'article_id' => 'required|integer'];
        $data = ['username' => $username, 'email' => $email, 'content' => $content, 'at' => $at, 'article_id' => $article_id];
        $validator = Validator::make($data, $rules);
        if ($validator->fails()) {
            //return Redirect::route('article.show', array(Input::get('article_id')))->withErrors($validator);
            return Redirect::back()->withInput()->withErrors($validator);
        }
        $comment = Comment::create($data);
        return Redirect::route('article.show', array($article_id));

    }

    /**
     * Display the specified resource.
     * GET /comment/{id}
     *
     * @param  int $id
     * @return Response
     */
    public function show($id)
    {
        //
    }

    /**
     * Show the form for editing the specified resource.
     * GET /comment/{id}/edit
     *
     * @param  int $id
     * @return Response
     */
    public function edit($id)
    {
        //
    }

    /**
     * Update the specified resource in storage.
     * PUT /comment/{id}
     *
     * @param  int $id
     * @return Response
     */
    public function update($id)
    {
        //
    }

    /**
     * Remove the specified resource from storage.
     * DELETE /comment/{id}
     *
     * @param  int $id
     * @return Response
     */
    public function destroy($id)
    {
        try {
            Comment::find($id)->delete();
            return Redirect::to(URL::previous());
        } catch (\Exception $e) {
            return Redirect::to(URL::previous());
        }

    }

}