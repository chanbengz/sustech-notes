#let project(
  title: "",
  authors: (),
  abstract: none,
  keywords: (),
  body
) = {
  let zh_kai = "Kaiti SC"
  let zh_hei = "Heiti SC"
  let zh_songti = "Songti SC"
  let en_sans_serif = "Georgia"
  let en_serif = "Times New Roman"
  let en_typewriter = "Courier New"
  let en_code = "Menlo"
  // Moidfy the following to change the font.
  let title-font = (en_serif, zh_hei)
  let author-font = (en_typewriter, zh_songti)
  let body-font = (en_serif, )
  let heading-font = (en_serif, )
  let caption-font = (en_serif, zh_kai)
  let header-font = (en_serif, zh_kai)
  let strong-font = (en_serif, zh_hei)
  let emph-font = (en_serif, zh_kai)
  let raw-font = (en_code, zh_hei)
  
  set document(author: authors.map(author => author.name), title: title)
  set page(numbering: "1", number-align: center, header: align(left)[
    #set text(font: header-font)
    #title
  ])
  set heading(numbering: "1.1")
  set text(font: body-font, lang: "en", region: "cn")
  set bibliography(style: "institute-of-electrical-and-electronics-engineers")
  
  show heading: it => box(width: 100%)[
    #v(0.50em)
    #set text(font: heading-font)
    #if it.numbering != none { counter(heading).display() }
    #h(0.75em)
    #it.body
]

  show heading.where(
    level: 1
  ): it => box(width: 100%)[
    #v(0.5em)
    #set align(center)
    #set heading(numbering: "一")
    #it
    #v(0.75em)
  ]

  // Title
  align(center)[
    #block(text(font: title-font, weight: "bold", 1.75em, title))
    #v(0.5em)
  ]

  // Display the authors list.
  for i in range(calc.ceil(authors.len() / 3)) {
    let end = calc.min((i + 1) * 3, authors.len())
    let is-last = authors.len() == end
    let slice = authors.slice(i * 3, end)
    grid(
      columns: slice.len() * (1fr,),
      gutter: 12pt,
      ..slice.map(author => align(center, {
        text(12pt, author.name, font: author-font)
        if "organization" in author [
          \ #text(author.organization, font: author-font)
        ]
        if "email" in author [
          \ #text(link("mailto:" + author.email), font: author-font)
        ]
      }))
    )

    if not is-last {
      v(16pt, weak: true)
    }
  }
  v(2em, weak: true)

  set par(first-line-indent: 2em, justify: true)
  set enum(indent: 2em)
  set list(indent: 2em)
  set figure(gap: 0.8cm)

  let blank_par = par()[#text()[#v(0em, weak: true)];#text()[#h(0em)]]

  show figure: it => [
    #v(12pt)
    #set text(font: caption-font)
    #it
    #blank_par
    #v(12pt)
  ]

  show image: it => [
    #it
    #blank_par
  ]

  show list: it => [
    #it
    #blank_par
  ]

  show enum: it => [
    #it
    #blank_par
  ]

  show table: it => [
    #set text(font: body-font)
    #it
    #blank_par
  ]
  show strong: set text(font: strong-font)
  show emph: set text(font: emph-font)
  show ref: set text(red)
  show raw.where(block: true): block.with(
    width: 100%,
    fill: luma(240),
    inset: 10pt,
  )

  show raw.where(block: true): it => [
    #it
    #blank_par
  ]

  show raw: set text(font: raw-font)
  show link: underline
  show link: set text(blue)
  body
}
#let problem-counter = counter("problem")
#problem-counter.step()

#let problem(body) = {
  problem-counter.step()
  set enum(numbering: "(1)")
  block(
    fill: rgb(241, 241, 255),
    inset: 8pt,
    radius: 2pt,
    width: 100%,
  )[*Problem #context problem-counter.display().* #h(0.75em) #body]
}

#let solution(body) = {
  set enum(numbering: "(1)")
  block(
    inset: 8pt,
    width: 100%
  )[*Solution.* #h(0.75em) #body]
}
