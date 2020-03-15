type state = {
  input: string,
  isLoading: bool,
  results: list(Github.repository),
};

type action =
  | Search
  | UpdateInput(string)
  | UpdateResults(list(Github.repository));

let initialState = {input: "", isLoading: false, results: []};

let reducer = (state, action) => {
  switch (action) {
  | Search => {...state, isLoading: true}
  | UpdateInput(input) => {...state, input}
  | UpdateResults(results) => {...state, isLoading: false, results}
  };
};

let value = ev => ReactEvent.Form.target(ev)##value;
let preventDefault = ev => ReactEvent.Form.preventDefault(ev);

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(reducer, initialState);

  let handleResults = results =>
    switch (results) {
    | None =>
      dispatch(UpdateResults([]));
      Js.Promise.resolve();
    | Some(results) =>
      dispatch(UpdateResults(results));
      Js.Promise.resolve();
    };

  let onSubmit = ev => {
    preventDefault(ev);
    dispatch(Search);
    let _ = Github.search(state.input) |> Js.Promise.then_(handleResults);
    ();
  };

  <div>
    <form onSubmit>
      <label htmlFor="search"> {React.string("Search")} </label>
      <input
        id="search"
        name="search"
        value={state.input}
        onChange={ev => dispatch(UpdateInput(value(ev)))}
      />
      <button type_="submit"> {React.string("Submit Search")} </button>
    </form>
    <div>
      {state.isLoading
         ? React.string("Loading...")
         : state.results
           ->Belt.List.toArray
           ->Belt.Array.map(({name, url, description}) =>
               <Card key=url name href=url description />
             )
           ->React.array}
    </div>
  </div>;
};