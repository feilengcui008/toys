@extends('common.default')
@section('blogs')
@if($article)
<div class="col-sm-8 blog-main">
	<hr />
	<div class="blog-post">
		<div class="blog-post-title">
			{{$article->title}}
		</div>
		<p class="blog-post-meta">
		{{date("Y/m/d",strtotime($article->updated_at))}}
		</p>
		<p class="blog-post-meta">
		@if(count($blogtagarray) > 0)
		@foreach($blogtagarray as $val)
		<a href="/tag/{{$val->id}}">
			<button type="button" class="btn btn-default btn-sm tag">
			<span class="glyphicon glyphicon-tag"></span> {{$val->name}}
		</button></a>
		@endforeach
		@endif
		</p>
		<p>
		{{$article->content}}
		</p>
		<p align="right"><button class="btn btn-info" onclick="javascript:window.scrollTo(0,99999)">新增回复</button></p>
	</div>
	<div>
	    @if($comments)
	        @foreach($comments as $comment)
	        <hr />
	            <p><span class="text-info">{{{$comment->username}}}</span>
	             @if($comment->at)
	                @<span class="text-info">{{{$comment->at}}}</span>
	             @endif
	             at {{{$comment->created_at}}}</p>
	            <p>{{{ $comment->content }}}</p>
	            <p align="right"><button class="btn btn-xs btn-info" onclick="javascript:reply({{{$comment->id}}},'{{{$comment->username}}}')">回复</button></p>

	        @endforeach
	    @endif



	</div>
	<div>
	{{ Form::open(array('route' => array('comment.store'), 'method' => 'post','id'=>'replyform')) }}
    			    @if ($errors)
    			    @foreach ($errors->all() as $message)
                          <span class="text-danger">{{ $message }}</span><br />
                    @endforeach
                    @endif

                    <input type="hidden" name="article_id" value="{{{$article->id}}}" />
                    <div class="control-group">
                        {{ Form::label('username', '用户名') }}
                        <div class="controls">
                        {{ Form::text('username') }}
                        </div>
                    </div>
                    <div class="control-group">
                        {{ Form::label('email', '邮箱') }}
                        <div class="controls">
                        {{ Form::text('email') }}
                        </div>
                    </div>
                    <div class="control-group">
                        {{ Form::label('content', '内容') }}
                        <div class="controls">
                        {{ Form::textarea('content') }}
                        </div>
                    </div>

                    <div class="form-actions">

                        {{ Form::submit('添加', array('class' => 'btn btn-info')) }}

                    </div>
                    <hr />
    			{{ Form::close() }}
	</div>
</div>
@else
somthing wrong!
@endif
@stop
@section('sidebar')
<div class="col-sm-3 blog-sidebar col-sm-offset-1">
	<hr />
	@include('common.tag')
	<hr />
	@include('common.archive')
</div>
@stop