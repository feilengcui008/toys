@include('common.header')
@include('admin.nav')
<div class="container">
	<hr />
	<div class="row">
		<div class="col-sm-12">
			<table class="table table-bordered table-hover">
				<h3>标签管理</h3>
				<p><a href="{{URL::route('tag.create')}}"><button class="btn btn-info">新增</button></a></p>

				<thead>

					<tr>
						<th>id</th>
						<th>name</th>
						<th>updated date</th>
						<th></th>
						<th></th>
					</tr>
				</thead>
				<tbody>
					@foreach($tags as $tag)
					<tr>
						<td>{{$tag->id}}</td>
						<td>{{$tag->name}}</td>
						<td>{{$tag->updated_at}}</td>
						<td><a href="/tag/{{$tag->id}}/edit">编辑</a></td>
						<td>
                        {{ Form::open(array('route' => array('tag.destroy', $tag->id), 'method' => 'delete')) }}
                        {{ Form::submit('删除', array('class' => 'btn btn-info')) }}
                        {{ Form::close() }}
                        </td>
					</tr>
					@endforeach
				</tbody>
			</table>
			{{$tags->links()}}
		</div>
	</div>
</div>
@include('common.footer')