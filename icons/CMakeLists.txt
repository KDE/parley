include(ECMInstallIcons)
ecm_install_icons( ICONS
  16-actions-cards-block.png        22-actions-lesson-remove.png      32-actions-practice-start.png
  22-actions-list-add-card.png      32-actions-practice-stop.png
  16-actions-edit-entry.png         22-actions-list-remove-card.png   32-actions-remove-duplicates.png
  16-actions-edit-table-row.png     22-actions-multiple-choice.png    32-actions-set-language.png
  16-actions-lesson-add.png         22-actions-practice-setup.png     32-actions-specific-setup.png
  16-actions-lesson-remove.png      22-actions-practice-start.png     48-actions-cards-block.png
  16-actions-list-add-card.png      22-actions-practice-stop.png      
  16-actions-list-remove-card.png   22-actions-remove-duplicates.png  48-actions-edit-entry.png
  16-actions-multiple-choice.png    22-actions-set-language.png       48-actions-edit-table-row.png
  16-actions-practice-setup.png     22-actions-specific-setup.png     48-actions-lesson-add.png
  16-actions-practice-start.png     32-actions-cards-block.png        48-actions-lesson-remove.png
  16-actions-practice-stop.png      48-actions-list-add-card.png
  16-actions-remove-duplicates.png  32-actions-edit-entry.png         48-actions-list-remove-card.png
  16-actions-set-language.png       32-actions-edit-table-row.png     48-actions-multiple-choice.png
  16-actions-specific-setup.png     32-actions-lesson-add.png         48-actions-practice-setup.png
  22-actions-cards-block.png        32-actions-lesson-remove.png      48-actions-practice-start.png
  32-actions-list-add-card.png      48-actions-practice-stop.png
  22-actions-edit-entry.png         32-actions-list-remove-card.png   48-actions-remove-duplicates.png
  22-actions-edit-table-row.png     32-actions-multiple-choice.png    48-actions-set-language.png
  22-actions-lesson-add.png         32-actions-practice-setup.png     48-actions-specific-setup.png

  sc-actions-cards-block.svgz     sc-actions-lesson-remove.svgz     sc-actions-remove-duplicates.svgz
  sc-actions-list-add-card.svgz   sc-actions-set-language.svgz
  sc-actions-edit-entry.svgz      sc-actions-list-remove-card.svgz  sc-actions-specific-setup.svgz
  sc-actions-edit-table-row.svgz  sc-actions-practice-setup.svgz
  sc-actions-lesson-add.svgz      sc-actions-practice-start.svgz

  DESTINATION ${KDE_INSTALL_ICONDIR}
  THEME oxygen
)

# Windows and OS X only
ecm_install_icons( ICONS
  128-apps-parley.png  32-apps-parley.png  64-apps-parley.png
  sc-apps-parley-simple.svgz 16-apps-parley.png   48-apps-parley.png
  sc-apps-parley-parley.svgz  sc-apps-parley.svgz

  DESTINATION ${KDE_INSTALL_ICONDIR}
  THEME hicolor
)
