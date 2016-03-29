<div class="sidebar-module">
	<h4>Tag</h4>
	<ol class="list-unstyled">
		@if($tags)
		@foreach($tags as $tag)
		<a href="/tag/{{$tag->id}}"
			<button type="button" class="btn btn-default btn-sm tag {{ (Request::is('tag/'.$tag->id) ? ' active' : '') }}">
			<span class="glyphicon glyphicon-tag"></span> {{$tag->name}}
			</button>
		</a>
		@endforeach
		@else
		<p>no tags yet</p>
		@endif
	</ol>
</div>