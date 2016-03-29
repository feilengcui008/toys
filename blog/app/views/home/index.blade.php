@extends('common.default')


@section('blogs')
@include('common.articles')
@stop


@section('sidebar')
<div class="col-sm-3 blog-sidebar col-sm-offset-1">
	<hr />
	@include('common.tag')
	<hr />
	@include('common.archive')
</div>
@stop
