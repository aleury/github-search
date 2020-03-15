module Styles = {
  open Css;
  let card =
    style([
      padding(rem(1.0)),
      borderRadius(px(4)),
      border(px(1), `solid, hex("898989")),
    ]);
};

[@react.component]
let make = (~name, ~description, ~href) => {
  <div className=Styles.card>
    <h3>
      <a href target="_blank" rel="noopener noreferrer">
        {React.string(name)}
      </a>
    </h3>
    <p> {React.string(description)} </p>
  </div>;
};