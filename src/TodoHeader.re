include TodoUtil;

let component = ReasonReact.statelessComponent("TodoHeader");

let make = (
	~newDescription,
	~onDescriptionChanged,
	~onAddTodo,
	_children) => {
	...component,
	render: (_self) =>
		<header className="header">
			<h1>(s("todos"))</h1>
			<input
				className="new-todo"
				placeholder="What needs to be done?"
				autoFocus=(Js.Boolean.to_js_boolean(true))
				onInput=((event) => onDescriptionChanged(getTargetValue(event)))
				onKeyDown=((event) =>
					if (ReactEventRe.Keyboard.key(event) == "Enter") {
						onAddTodo()
					}
				)
				value=(newDescription)
			/>
		</header>
};
