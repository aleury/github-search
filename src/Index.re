// Entry point

[@bs.val] external document: Js.t({..}) = "document";

ReactDOMRe.renderToElementWithId(<App />, "root");