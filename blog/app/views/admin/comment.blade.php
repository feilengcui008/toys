@include('common.header')
@include('admin.nav')
<div class="container">
	<hr />
	<div class="row">
		<div class="col-sm-12">
			<table class="table table-bordered">
				<h3>评论管理</h3>

				<span>文章ID：{{$data->id}}</span><br/>
				<span>文章标题：{{$data->title}}</span>
				<hr/>

				<thead>

					<tr>
						<th>username</th>
						<th>email</th>
						<th>内容</th>
						<th>评论时间</th>
						<th>@</th>
						<th>操作</th>
					</tr>
				</thead>
				<tbody>
					@foreach($data->comments as $comment)
					<tr>
						<td>{{$comment->username}}</td>
						<td>{{$comment->email}}</td>
						<td>{{$comment->content}}</td>
						<td>{{$comment->created_at}}</td>
						<td>{{$comment->at}}</td>
						<td>
						{{ Form::open(array('route' => array('comment.destroy', $comment->id), 'method' => 'delete')) }}
					    {{ Form::submit('删除', array('class' => 'btn btn-info')) }}
					    {{ Form::close() }}
					    </td>
					</tr>
					@endforeach
				</tbody>
			</table>
			<hr/>
		</div>
	</div>
</div>
@include('common.footer')