@include('common.header')
@include('admin.nav')
<div class="container">
	<hr />
	<div class="row">
		<div class="col-sm-12">
			<table class="table table-bordered table-hover">
				<h3>文章管理</h3>
				<p><a href="{{URL::route('article.create')}}"><button class="btn btn-info">新增</button></a></p>

				<thead>

					<tr>
						<th>id</th>
						<th>titie</th>
						<th>updated date</th>
						<th></th>
						<th></th>
						<th></th>
					</tr>

				</thead>
				<tbody>
					@foreach($articles as $article)
					<tr>
						<td>{{$article->id}}</td>
						<td>{{$article->title}}</td>
						<td>{{$article->updated_at}}</td>
						<td><a href="/article/{{$article->id}}/edit">编辑</a></td>
						<td>
                        {{ Form::open(array('route' => array('article.destroy', $article->id), 'method' => 'delete')) }}
                        {{ Form::submit('删除', array('class' => 'btn btn-info')) }}
                        {{ Form::close() }}
                        </td>
						<td><a href="/admin/comment/{{$article->id}}">评论管理</a></td>
					</tr>
					@endforeach
				</tbody>
			</table>
			{{$articles->links()}}
		</div>
	</div>
	
</div>
@include('common.footer')
