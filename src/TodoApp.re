type todoItem = {
	id: int,
	description: string,
	completed: bool,
	editing: bool
};

type visibility = All | Active | Completed;

type action =
	| AddTodo
	| UpdateNewDescription(string)
	| ToggleCompleted(int)
	| ToggleEditing(int)
	| Remove(int)
	| DescriptionChanged(int, string)
	| ClearCompleted
	| Clear
	| ToggleAllCompleted
	| UpdateVisibility(visibility);

type state = {
	items: list(todoItem),
	newDescription: string,
	nextId: int,
	visibility: visibility
};

let component = ReasonReact.reducerComponent("TodoApp");

let s = ReasonReact.stringToElement;
let getTargetValue = event => ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value;

let numRemainingTodos = state =>
	List.length(
		List.filter(
			(item) => !item.completed,
			state.items
		)
	);
let filteredItems = (items, visibility) => {
	switch visibility {
	| All => items
	| Active => List.filter(i => !i.completed, items)
	| Completed => List.filter(i => i.completed, items)
	}
};

let make = (_children) => {
	...component,
	initialState: () => {
		items: [
			{ id: 0, description: "Taste ReasonML", completed: true, editing: false },
			{ id: 1, description: "Buy a unicorn", completed: false, editing: false }
		],
		newDescription: "",
		nextId: 2,
		visibility: All
	},
	reducer: (action, state) =>
		switch action {
		| AddTodo => ReasonReact.Update({
			...state,
			items: List.append(
				state.items,
				[{ id: state.nextId, description: state.newDescription, completed: false, editing: false }]
			),
			newDescription: "",
			nextId: state.nextId + 1
		})

		| Clear => ReasonReact.Update({...state, items: [] })
		| UpdateNewDescription(newDescription) => ReasonReact.Update({ ...state, newDescription })
		| ToggleCompleted(id) => ReasonReact.Update({
				...state,
				items: List.map((item) =>
					item.id == id
						? { ...item, completed: !item.completed }
						: item,
					state.items
				)
			})
		| ToggleEditing(id) =>
				ReasonReact.Update({
					...state,
					items: List.map(item =>
						item.id == id
							? { ...item, editing: !item.editing }
							: item,
						state.items
					)
				})
		| DescriptionChanged(id, description) =>
				ReasonReact.Update({
					...state,
					items: List.map(item =>
						item.id == id
							? { ...item, description }
							: item,
						state.items
					)
				})
		| ClearCompleted =>
				ReasonReact.Update({
					...state,
					items: List.filter((item) => !item.completed, state.items)
				})
		| ToggleAllCompleted =>
				ReasonReact.Update({
					...state,
					items: List.for_all(i => i.completed, state.items)
						? List.map(i => { ...i, completed: false }, state.items)
						: List.map(i => { ...i, completed: true }, state.items)
				});
		| Remove(id) =>
				ReasonReact.Update({
					...state,
					items: List.filter(i => i.id != id, state.items)
				})
		| UpdateVisibility(visibility) => ReasonReact.Update({ ...state, visibility })
		},
	render: ({ state: state, reduce }) =>
		<div>
			<section className="todoapp">
				<header className="header">
					<h1>(s("todos"))</h1>
					<input
						className="new-todo"
						placeholder="What needs to be done?"
						autoFocus=(Js.Boolean.to_js_boolean(true))
						onInput=((event) =>
							(reduce(() =>
									UpdateNewDescription(getTargetValue(event))
							))())
						onKeyDown=((event) =>
							if (ReactEventRe.Keyboard.key(event) == "Enter") {
								(reduce(() => AddTodo))()
							}
						)
						value=(state.newDescription)
					/>
				</header>
				/* This section should be hidden by default and shown when there are todos */
				<section className="main">
					<input
						id="toggle-all"
						className="toggle-all"
						_type="checkbox"
						checked=(Js.Boolean.to_js_boolean(List.length(state.items) > 0 && List.for_all(i => i.completed, state.items)))
						onChange=((_) => reduce(() => ToggleAllCompleted)())
					/>
					<label htmlFor="toggle-all">(s("Mark all as complete"))</label>
					<ul className="todo-list">
						(ReasonReact.arrayToElement(Array.of_list(
							List.map(item =>
								<TodoItem
									completed=(item.completed)
									description=(item.description)
									editing=(item.editing)
									onToggleCompleted=(reduce(() => ToggleCompleted(item.id)))
									onToggleEditing=(reduce(() => ToggleEditing(item.id)))
									onDescriptionChanged=(
										(description) => {
											(reduce(() => DescriptionChanged(item.id, description))())
										}
									)
									onRemove=(reduce(() => Remove(item.id)))
								/>,
								filteredItems(state.items, state.visibility)
							)
						)))
					</ul>
				</section>
				/*- This footer should hidden by default and shown when there are todos */
				(List.length(state.items) > 0
					? <footer className="footer">
							/* This should be `0 items left` by default */
							<span className="todo-count">
								<strong>(s(Js.Int.toString(numRemainingTodos(state))))</strong>
								(numRemainingTodos(state) == 1 ? s(" item left") : s(" items left"))
							</span>
							/* Remove this if you don't implement routing */
							<ul className="filters">
								<li onClick=(_e => reduce(() => UpdateVisibility(All))())>
									<a className=(state.visibility == All ? "selected" : "") href="#/">(s("All"))</a>
								</li>
								<li onClick=(_e => reduce(() => UpdateVisibility(Active))())>
									<a className=(state.visibility == Active ? "selected" : "") href="#/active">(s("Active"))</a>
								</li>
								<li onClick=(_e => reduce(() => UpdateVisibility(Completed))())>
									<a className=(state.visibility == Completed ? "selected" : "") href="#/completed">(s("Completed"))</a>
								</li>
							</ul>
							/* Hide if no items left */
							<button className="clear-completed" onClick=((_event) => (reduce(() => ClearCompleted)()))>(s("Clear completed"))</button>
						</footer>
				: ReasonReact.nullElement)
			</section>
			<footer className="info">
				<p>(ReasonReact.stringToElement("Double-click to edit a todo"))</p>
				<p>(ReasonReact.stringToElement("Created by "))<a href="http://github.com/bspaulding">(s("Bradley Spaulding"))</a></p>
				<p>(ReasonReact.stringToElement("Part of "))<a href="http://todomvc.com">(s("TodoMVC"))</a></p>
			</footer>
		</div>
};
