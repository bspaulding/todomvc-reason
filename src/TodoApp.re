include TodoAppReducer;
include Selectors;

let component = ReasonReact.reducerComponent("TodoApp");

let s = ReasonReact.stringToElement;
let getTargetValue = event => ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value;


let make = (_children) => {
	...component,
	initialState,
	reducer,
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
										description => {
											(reduce(() => DescriptionChanged(item.id, description))())
										}
									)
									onRemove=(reduce(() => Remove(item.id)))
								/>,
								Selectors.filteredItems(state.items, state.visibility)
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
