export const ACTION_COLORS = {
  eating:   '#f28c8c',  // bright soft coral
  thinking: '#8cc9f2',  // light azure blue
  sleeping: '#f2e28c',  // creamy pastel yellow
  died:     '#c2bfc9',  // soft lilac-gray
  default:  '#b0d6e8'   // pastel sky blue
};

export function getActionColor(type) {
  return ACTION_COLORS[type] || ACTION_COLORS.default;
}
