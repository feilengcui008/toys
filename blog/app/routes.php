<?php

/*
|--------------------------------------------------------------------------
| Application Routes
|--------------------------------------------------------------------------
|
| Here is where you can register all of the routes for an application.
| It's a breeze. Simply tell Laravel the URIs it should respond to
| and give it the Closure to execute when that URI is requested.
|
*/

Route::pattern('id', '[0-9]+');

Route::get('/', array('as' => 'home', 'uses' => 'HomeController@index'));
Route::get('about', array('as' => 'about', 'uses' => 'HomeController@about'));
Route::resource('article', 'ArticleController');
Route::resource('tag', 'TagController');
Route::resource('comment', 'CommentController');


Route::get('logout', array('as' => 'admin.logout', 'uses' => 'AuthController@getLogout'));
Route::get('login', array('as' => 'admin.login', 'uses' => 'AuthController@getLogin'));
Route::post('login', array('as' => 'admin.login.post', 'uses' => 'AuthController@postLogin'));


Route::when('admin*', 'admin');
Route::get('admin', array('as' => 'admin.index', 'uses' => 'AdminController@index'));
Route::get('admin/tag', array('as' => 'admin.tag', 'uses' => 'AdminController@tagAdmin'));
Route::get('admin/comment/{id}', array('as' => 'admin.comment', 'uses' => 'AdminController@commentAdmin'));

Route::get('test', 'HomeController@test');

